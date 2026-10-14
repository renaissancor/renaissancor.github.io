---
title: "I/O Completion Ports (IOCP) for Socket Servers"
---

# I/O Completion Ports (IOCP) for Socket Servers

[win-04](win-04-async-io-and-iocp.md) built up Overlapped I/O: a request is
issued, returns at once, and the OS finishes it in the background. It covered
two ways to learn that a request finished: an event object and a completion
routine. Both break down once a server has thousands of sockets. IOCP is the
third way, and it is the one most high-performance Windows servers and game
backends are built on.

This post covers IOCP as the core of a socket server. Part 1 is the
mechanism. Part 2 is what a real server has to handle on top of it, which the
API reference does not say plainly. In the I/O models series,
IOCP is the Windows **completion** model: the kernel tells you an operation
has *finished*, not that a socket is *ready*.

---

## Part 1 — The mechanism

### The scalability problem IOCP solves

Overlapped I/O with events or completion routines leaves two bad options:

- **One thread per connection**: 10,000 connections means 10,000 threads,
  each with its own stack, and a scheduler thrashing between them.
- **A fixed pool waiting on events**: `WaitForMultipleObjects` takes at most
  64 handles, so the pool has to shard events across threads and still polls.

IOCP replaces both with one kernel object: a **completion port**. It is a
queue that collects completion notifications from any number of handles. A
small pool of worker threads blocks on it in `GetQueuedCompletionStatus`. The
kernel decides how many of them run at once.

### Creating a port and associating sockets

```cpp
// Step 1: create the port
HANDLE hIOCP = CreateIoCompletionPort(
    INVALID_HANDLE_VALUE,   // no handle yet
    NULL,                   // create a new port
    0,                      // completion key (unused here)
    0                       // concurrency value: 0 = number of processors
);

// Step 2: associate each socket with the port
CreateIoCompletionPort(
    (HANDLE)hSocket,        // created with WSA_FLAG_OVERLAPPED
    hIOCP,
    (ULONG_PTR)pSession,    // completion key: your per-socket context
    0
);
```

The **completion key** is an opaque value that the OS hands back with every
completion on that handle. Put a pointer to the per-connection state there.

### Per-handle and per-I/O context

A server keeps two kinds of context:

| | per-handle (per-session) | per-I/O (per-operation) |
|---|---|---|
| lives as long as | the connection | one outstanding operation |
| carried by | the completion key | the `OVERLAPPED` pointer |
| holds | socket, session state, send queue | `OVERLAPPED`, `WSABUF`, buffer, operation type (recv / send / accept) |

The `OVERLAPPED` is embedded in the per-I/O struct, and the struct is
recovered from the pointer GQCS returns:

```cpp
struct IoContext {
    OVERLAPPED ov;          // the OS sees only this
    enum Op { Recv, Send, Accept } op;
    WSABUF     wsabuf;
    char       buf[4096];
};

IoContext* io = CONTAINING_RECORD(pOv, IoContext, ov);
```

### Issuing a request

```cpp
DWORD flags = 0;
int rc = WSARecv(s->sock, &io->wsabuf, 1, NULL, &flags, &io->ov, NULL);
if (rc == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
    // a real failure; no completion will arrive for this request
}
```

`WSA_IO_PENDING` is not an error. It means the OS accepted the request.
**From here until the completion is dequeued, the OS owns `io` and its
buffer.** Freeing or reusing either before then is the classic IOCP
use-after-free.

### The worker loop, with errors handled correctly

```cpp
for (;;) {
    DWORD        bytes = 0;
    ULONG_PTR    key   = 0;
    LPOVERLAPPED pOv   = NULL;

    BOOL ok = GetQueuedCompletionStatus(hIOCP, &bytes, &key, &pOv, INFINITE);

    if (!ok) {
        if (pOv == NULL) {
            // No packet was dequeued: the port itself failed (or a timeout,
            // if one was given). Nothing to clean up per operation.
            break;
        }
        // A packet WAS dequeued, but that I/O failed: reset connection,
        // cancelled by closesocket (ERROR_OPERATION_ABORTED), and so on.
        // The per-I/O context must still be released.
        on_io_failed((Session*)key, CONTAINING_RECORD(pOv, IoContext, ov));
        continue;
    }

    IoContext* io = CONTAINING_RECORD(pOv, IoContext, ov);
    if (io->op == IoContext::Recv && bytes == 0) {
        // Graceful close by the peer. Only a RECV with 0 bytes means this;
        // a 0-byte completion on other operations means something else.
        on_disconnect((Session*)key, io);
        continue;
    }
    dispatch((Session*)key, io, bytes);
}
```

`ok == FALSE` covers two different cases, and `pOv` is what tells them
apart. Treating every `bytes == 0` as a disconnect is wrong for anything that
is not a receive.

### What the concurrency value actually limits

The last argument of `CreateIoCompletionPort` limits how many threads
associated with the port may be **running** at once, not how many exist.

- When a worker **blocks** for another reason (a lock, a synchronous call, a
  page fault), it stops counting as running, and the kernel may release
  another waiting worker. That is why pools are usually larger than the core
  count (2 × cores is the common rule of thumb): the extra threads cover
  workers that block.
- Waiting threads are released in **LIFO** order, while packets leave the
  queue in **FIFO** order: the most recently blocked thread gets the oldest
  completion. That thread's stack and cache are still warm, and threads at
  the bottom of the stack stay asleep (Microsoft, *I/O Completion Ports*).
- A thread can be associated with at most one completion port.

So the kernel does not simply "never wake more threads than CPUs". It keeps
the number of *running* workers near the concurrency value.

### Architecture summary

```
  many sockets ── overlapped WSARecv / WSASend / AcceptEx
                         │  completions queued by the OS
                         ▼
               ┌──────────────────────┐
               │   completion port    │  kernel queue, one per server
               └──────────────────────┘
                         │  GQCS: running workers ≈ concurrency value,
                         ▼        released LIFO
         worker 1 ── worker 2 ── … ── worker N    (any worker, any completion)
```

---

## Part 2 — What a server has to handle

### Any worker gets any completion: the ordering problem

This is the property that shapes every IOCP server. **A completion goes to
whichever worker is free, not to a worker tied to its socket.**

- **Receives.** Keep **one outstanding `WSARecv` per socket** and re-post it
  only after handling the completion. Two receives in flight on one socket
  can complete on two workers, and the stream is then reassembled out of
  order.
- **Sends.** Several sends in flight on one socket can complete on different
  workers in any order. Keep **one outstanding `WSASend` per session**,
  queue the rest, and send the queue with a `WSABUF` array (scatter/gather)
  when the current send completes.
- **Session state.** Even with one recv and one send in flight, the recv
  completion and the send completion of one session can run **at the same
  time on two workers.** Session state therefore needs a lock or a
  serialization mechanism.

This is why IOCP servers grow per-session locks, per-session or per-room job
queues ("strands"), or hand parsed packets to a dedicated logic thread. The
I/O pool is fungible by design, so ownership of state has to be rebuilt above
it. Linux's `io_uring` takes the opposite approach. A ring belongs to one
thread and completions come back to that thread, so a server can make the
I/O thread and the state owner the same thread. That contrast is the subject
of the io_uring post in this series.

### Accepting connections: `AcceptEx`

A blocking `accept` loop on a separate thread works, but it serialises
accepts and does not use the port. `AcceptEx` makes accept an overlapped
operation:

- **Pre-create sockets and pre-post several `AcceptEx` calls**, so a burst of
  connections finds accepts already waiting.
- **Fetch the function pointer once** with
  `WSAIoctl(SIO_GET_EXTENSION_FUNCTION_POINTER, WSAID_ACCEPTEX)`.
  `ConnectEx` and `DisconnectEx` come the same way.
- **After completion, call `setsockopt(SO_UPDATE_ACCEPT_CONTEXT)`** on the
  accepted socket, or functions such as `getpeername` and `shutdown` will not
  behave. Then read the addresses with `GetAcceptExSockaddrs`.
- **`DisconnectEx(TF_REUSE_SOCKET)`** lets a closed socket be reused for the
  next `AcceptEx`, which saves socket creation under churn.

### Locked pages, and the zero-byte receive

The OS locks the pages of a buffer while an overlapped operation on it is
outstanding. It is tempting to read this as "the NIC DMAs straight into your
buffer, so there is no copy". That is not how TCP works. Received segments
are processed by the kernel's TCP/IP stack and the payload is copied into
your buffer. Pinning guarantees the buffer stays resident for that copy; it
does not remove the copy.

The pinning has a cost of its own. **10,000 sockets × one outstanding 4 KB
receive is 40 MB of locked memory, held while the sockets sit idle.** Locked
pages come out of a limited system resource. A server that pins too much
starts seeing I/O requests fail, and it competes with everything else on the
machine.

The standard trick is the **zero-byte receive**:

1. Post `WSARecv` with a zero-length buffer. Nothing is locked while the
   socket is idle.
2. When it completes, data is available. Read it with a non-blocking
   `recv` into a real buffer, looping until `WSAEWOULDBLOCK`.
3. Post the zero-byte receive again.

It costs one extra call per wake-up, in exchange for locked memory that
scales with active connections instead of all connections.

### Fewer kernel transitions: batching and skipping the port

- **`GetQueuedCompletionStatusEx`** dequeues up to *N* completions in one
  call instead of one per call. Under load this cuts the number of kernel
  transitions per completion.
- **`SetFileCompletionNotificationModes(FILE_SKIP_COMPLETION_PORT_ON_SUCCESS)`**:
  - When an operation completes immediately, no packet is queued to the port
    and the caller handles the result inline. This saves a queue round trip
    per fast operation.
  - Every call site must then handle two completion paths, inline and via
    the port.
  - **For sockets, check the Winsock catalog first.** With a non-IFS Layered
    Service Provider installed, a socket with this mode set may never
    receive completions on its port (Microsoft KB 2568167). Enable the mode
    only if every entry from `WSAEnumProtocols` has `XP1_IFS_HANDLES` set in
    `dwServiceFlags1`. Chromium and the Go runtime both do this check, and
    both treat the mode as an optional optimisation.

### Lifetime and shutdown

- **Closing a socket** causes its outstanding operations to complete with an
  error, typically `ERROR_OPERATION_ABORTED`. Those completions still arrive
  and still carry your per-I/O contexts.
- **Free the session only after its last outstanding operation has
  completed.** In practice: a reference count per session, incremented per
  posted operation and decremented per dequeued completion. `CancelIoEx`
  speeds up cancellation; it does not remove the need to wait.
- **Stopping the workers**: post one sentinel packet per worker with
  `PostQueuedCompletionStatus` (for example a key of 0 with a NULL
  `OVERLAPPED`). Each worker exits when it dequeues one.
- `PostQueuedCompletionStatus` is also how a server pushes **its own jobs**
  into the same queue, which turns the port into a general thread-pool queue.

---

## Progression at a glance

| Model | Notification | Who handles the completion | Limit |
|---|---|---|---|
| Overlapped + event | `WaitFor…Object(s)` | the waiting thread | 64 handles per wait |
| Overlapped + completion routine (APC) | callback, during an alertable wait | the **issuing** thread | I/O pinned to its issuing thread |
| **IOCP** | `GetQueuedCompletionStatus(Ex)` | **any** worker of the pool | ordering and session state must be rebuilt above it |
| RIO | request and completion queues, registered buffers | (later post) | — |

---

*Part 1 extends the IOCP section of [win-04](win-04-async-io-and-iocp.md),
which follows 윤성우, 《윈도우즈 시스템 프로그래밍》. Part 2 is server practice
added on top of it.*
