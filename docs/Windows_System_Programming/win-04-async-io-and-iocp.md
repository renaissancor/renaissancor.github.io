---
title: "Windows File and Asynchronous I/O (Overlapped I/O & IOCP)"
---

# Windows File and Asynchronous I/O (Overlapped I/O & IOCP)

File I/O in Windows starts with three kernel-object-based primitives, graduates to
non-blocking Overlapped I/O, and ultimately reaches I/O Completion Ports (IOCP) —
the model that powers most high-performance Windows servers and game backends.
This post traces that progression end-to-end.

---

## Basic File I/O: CreateFile / ReadFile / WriteFile

### CreateFile

`CreateFile` is the universal entry point for files, pipes, serial ports, and other
I/O objects. Calling it asks the OS to create a **File Kernel Object** and returns
a `HANDLE`; no actual disk file is necessarily created.

```cpp
HANDLE CreateFile(
    LPCTSTR lpFileName,           // path
    DWORD   dwDesiredAccess,      // GENERIC_READ | GENERIC_WRITE
    DWORD   dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD   dwCreationDisposition, // CREATE_ALWAYS, OPEN_EXISTING, …
    DWORD   dwFlagsAndAttributes,  // FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED
    HANDLE  hTemplateFile
);
```

The `dwFlagsAndAttributes` field is critical later: passing `FILE_FLAG_OVERLAPPED`
opts the handle into asynchronous mode.  Without that flag the handle is
synchronous and Overlapped I/O calls will behave as blocking calls.

### ReadFile and WriteFile

```cpp
BOOL ReadFile(
    HANDLE       hFile,
    LPVOID       lpBuffer,
    DWORD        nNumberOfBytesToRead,
    LPDWORD      lpNumberOfBytesRead,  // NULL in async mode
    LPOVERLAPPED lpOverlapped          // NULL for synchronous
);

BOOL WriteFile(
    HANDLE       hFile,
    LPCVOID      lpBuffer,
    DWORD        nNumberOfBytesToWrite,
    LPDWORD      lpNumberOfBytesWritten, // NULL in async mode
    LPOVERLAPPED lpOverlapped
);
```

Return value is zero on failure; call `GetLastError()` for the code.
For async calls, `GetLastError()` returning `ERROR_IO_PENDING` is **not** an error
— it means the operation was accepted and will complete asynchronously.

### File Pointer (SetFilePointer)

On 32-bit-addressed files the low 32 bits are passed directly:

```cpp
SetFilePointer(hFile, sizeof(TCHAR) * 4, NULL, FILE_BEGIN);
```

For files larger than 4 GB, pass a `LONG*` for the high word and check both the
return value and `GetLastError()` to distinguish a genuine error from the valid
`INVALID_SET_FILE_POINTER` sentinel:

```cpp
DWORD dwPtrLow = SetFilePointer(hFile, lDistanceLow, &lDistanceHigh, FILE_BEGIN);
if (dwPtrLow == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR) {
    // real error
}
```

Other useful metadata functions: `GetFileTime`, `GetFileAttributes`,
`GetFileInformationByHandle`. All take a `HANDLE`; `GetFileAttributes` can also
work on a path string.  Close every handle with `CloseHandle`.

---

## Why Synchronous I/O Is Insufficient

I/O hardware operates largely independently of the CPU.  A `ReadFile` that blocks
the calling thread until the disk transfer completes wastes CPU time that could
serve other requests.  In server workloads where ~60% of operations are I/O, the
picture looks like this:

```
ReadFile call         ReadFile returns
    |                      |
    |<-- thread blocked --->|
                           |
                           v  (thread can finally do other work)
```

The CPU sits idle during the transfer.  For a video player this produces a
perceptible gap; for a server handling thousands of concurrent connections it
becomes a fundamental scalability ceiling.  The solution is **non-blocking,
asynchronous I/O**.

---

## Overlapped I/O

### The Core Idea

When a handle is opened with `FILE_FLAG_OVERLAPPED` and a non-NULL `LPOVERLAPPED`
is passed to `ReadFile`/`WriteFile`, the call **returns immediately** (usually with
`ERROR_IO_PENDING`), and the OS finishes the transfer in the background.  Multiple
I/O operations can be in-flight simultaneously on the same thread — they *overlap*:

```
Issue read A  ----[transfer A in progress]---->  complete
  Issue read B   ----[transfer B in progress]-------->  complete
    Issue read C      ----[transfer C in progress]----------->  complete
```

### The OVERLAPPED Structure

Every outstanding async operation must own one `OVERLAPPED` instance.  The OS
writes status back into it, so the buffer **must remain valid** for the lifetime of
the operation.

```cpp
typedef struct _OVERLAPPED {
    ULONG_PTR Internal;      // OS-internal status code
    ULONG_PTR InternalHigh;  // OS-internal bytes transferred
    DWORD     Offset;        // file offset low  (union with Pointer)
    DWORD     OffsetHigh;    // file offset high (union with Pointer)
    HANDLE    hEvent;        // optional event handle for notification
} OVERLAPPED;
```

`Offset`/`OffsetHigh` specify **where in the file** the operation reads or writes.
Because the file pointer concept is per-handle (and therefore shared across
overlapping calls), overlapped I/O bypasses it entirely by embedding the position
in the structure.

### Opening a Handle for Overlapped I/O

```cpp
HANDLE hFile = CreateFile(
    "data.bin",
    GENERIC_READ | GENERIC_WRITE,
    0, NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
    NULL
);
```

---

## Completion Notification Models

After issuing an overlapped request the thread must eventually learn when it
finished.  Windows provides three models.

### Model 1 — Event Object

Set `hEvent` in the `OVERLAPPED` struct to a manual-reset event, then wait on it:

```cpp
OVERLAPPED ov = {};
ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
ov.Offset = 0;

ReadFile(hFile, buf, sizeof(buf), NULL, &ov);

// do other CPU work here …

WaitForSingleObject(ov.hEvent, INFINITE);
CloseHandle(ov.hEvent);
```

`WaitForSingleObject` (or `WaitForMultipleObjects` for many events) blocks until
the event is signalled.  This is straightforward but each outstanding operation
consumes an event kernel object, and `WaitForMultipleObjects` caps out at 64
handles.

### Model 2 — Completion Routine (APC)

`ReadFileEx` / `WriteFileEx` accept a **completion routine** callback instead of
using the event field:

```cpp
BOOL ReadFileEx(
    HANDLE                          hFile,
    LPVOID                          lpBuffer,
    DWORD                           nNumberOfBytesToRead,
    LPOVERLAPPED                    lpOverlapped,
    LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
);
```

The callback signature:

```cpp
VOID CALLBACK FileIOCompletionRoutine(
    DWORD        dwErrorCode,
    DWORD        dwNumberOfBytesTransferred,
    LPOVERLAPPED lpOverlapped
);
```

Because the `hEvent` field is unused by the OS in this model, it is free to carry
an application-defined value (e.g. a pointer to a per-request context).

The callback is **not** called on an arbitrary thread at an arbitrary time.  It is
posted to the calling thread's **APC queue** (Asynchronous Procedure Call queue —
every thread has one), and the OS delivers it only when that thread enters an
**alertable wait state**:

```cpp
// Any of these puts the thread in alertable state:
SleepEx(INFINITE, TRUE);                 // second arg = bAlertable
WaitForSingleObjectEx(hEvent, ms, TRUE);
WaitForMultipleObjectsEx(n, h, FALSE, ms, TRUE);
```

When the thread is alertable and there are entries in the APC queue, Windows
drains the queue — calling each registered completion routine — before resuming
normal execution.

**Trade-off**: The completion routine always runs on the thread that *issued* the
I/O call.  For simple single-threaded programs that is convenient.  For a server
with many threads it couples I/O dispatch tightly to the issuing thread, making
load balancing harder.

---

## I/O Completion Ports (IOCP)

### The Scalability Problem IOCP Solves

Naive approaches to concurrent I/O assign one thread per connection (100
connections → 100 threads) or use a fixed pool but poll for completions.  Neither
scales: 100 threads thrash the scheduler with context switches; polling burns CPU.

IOCP is the OS-managed solution.  A single **completion port** — a kernel queue —
collects completion notifications from any number of handles.  A small pool of
worker threads (typically `2 × CPU cores`) each block in
`GetQueuedCompletionStatus`; the kernel wakes exactly as many threads as there are
available CPUs, eliminating over-subscription.

Additional OS-level optimizations kick in for IOCP-associated handles:
- **Page locking**: The user-mode buffer is pinned (locked in physical memory) for
  the duration of the transfer.  The NIC or disk DMA controller writes directly
  into it — no kernel-to-user copy.
- **Thread pool management**: The kernel tracks how many IOCP threads are actively
  running vs. waiting, and throttles wake-ups to prevent CPU over-subscription even
  when many completions arrive simultaneously.

### Creating and Associating a Completion Port

```cpp
// Step 1: create the port (first call — no handle to associate yet)
HANDLE hIOCP = CreateIoCompletionPort(
    INVALID_HANDLE_VALUE,   // no file handle yet
    NULL,                   // no existing port
    0,                      // completion key (unused)
    0                       // 0 = number of threads = number of CPUs
);

// Step 2: associate an I/O handle with the port
CreateIoCompletionPort(
    hFile,          // or hSocket opened with WSASocket(... WSA_FLAG_OVERLAPPED)
    hIOCP,
    (ULONG_PTR)pPerHandleData,  // completion key — your context pointer
    0
);
```

The **completion key** is an opaque value that the OS echoes back with every
completion on that handle.  Store a pointer to per-connection state there.

### Issuing an Overlapped Request

Issue reads or writes exactly as with plain Overlapped I/O.  For network sockets
use the Winsock equivalents:

```cpp
WSARecv(hSocket, &wsaBuf, 1, NULL, &flags, &perIoData->ov, NULL);
```

`perIoData->ov` is the `OVERLAPPED` struct embedded in a per-operation context
object.  After the call returns `WSA_IO_PENDING` the OS owns the buffer until the
completion is dequeued.

### Worker Thread Loop

```cpp
DWORD   bytesTransferred;
ULONG_PTR completionKey;
LPOVERLAPPED pOv;

while (true) {
    BOOL ok = GetQueuedCompletionStatus(
        hIOCP,
        &bytesTransferred,
        &completionKey,
        &pOv,
        INFINITE
    );

    if (!ok || bytesTransferred == 0) {
        // connection closed or error — clean up
        continue;
    }

    // Recover per-operation context from the OVERLAPPED pointer
    PerIoData* pIo = CONTAINING_RECORD(pOv, PerIoData, ov);

    // Process pIo, then re-issue WSARecv / WSASend …
}
```

`GetQueuedCompletionStatus` (GQCS) blocks until a completion packet arrives, then
returns the byte count, the completion key, and a pointer to the originating
`OVERLAPPED`.  Because `OVERLAPPED` is typically the first field of a larger
per-operation struct, `CONTAINING_RECORD` (or a `static_cast` from a known layout)
recovers the full context.

### IOCP Architecture Summary

```
  Client connections (many)
         │
         ▼
  Overlapped WSARecv / WSASend
         │  (completions posted automatically by OS)
         ▼
  ┌──────────────────────┐
  │   I/O Completion     │  ← kernel-managed queue
  │       Port           │
  └──────────────────────┘
         │  (GQCS wakes worker threads)
         ▼
  Worker Thread 1 ──┐
  Worker Thread 2 ──┤  (pool size ≈ 2 × CPU cores)
  Worker Thread N ──┘
```

The kernel never wakes more worker threads than there are logical CPUs, even if
hundreds of completions arrive at once.  Threads that complete their work quickly
return to GQCS and are recycled immediately, keeping the active thread count near
the hardware optimum.

---

## Progression at a Glance

| Model | Blocks caller? | Notification | Best for |
|---|---|---|---|
| Synchronous (`ReadFile`, no `OVERLAPPED`) | Yes | Return value | Simple sequential code |
| Overlapped + Event | No | `WaitForSingleObject` | Few concurrent operations |
| Overlapped + Completion Routine (APC) | No (alertable wait) | Callback on issuing thread | Single-thread async loops |
| IOCP | No | `GetQueuedCompletionStatus` | Many connections, scalable servers |

---

*Study notes based on 윤성우, 《윈도우즈 시스템 프로그래밍》 (book + lectures).*
