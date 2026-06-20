---
title: "Windows Inter-Process Communication (IPC)"
---

# Windows Inter-Process Communication (IPC)

Every Win32 process lives in its own isolated virtual address space — data, heap, and stack are never shared across process boundaries by design. IPC is the set of mechanisms the OS provides to let processes exchange data despite that isolation. Windows offers several first-class IPC primitives, each implemented as a kernel object accessed through a HANDLE and governed by the same object lifetime and usage-count rules.

---

## Kernel Objects, HANDLEs, and Usage Counts

Before covering individual IPC mechanisms, understanding the HANDLE model is essential because every IPC resource is managed through it.

When you call a creation function (e.g. `CreateMailslot`, `CreateFile`, `CreateFileMapping`), the kernel performs these steps:

1. Allocates the underlying resource (pipe buffer, shared memory region, etc.)
2. Creates a kernel object to represent it
3. Registers an entry in the calling process's **handle table** — a per-process mapping of handle values to kernel object addresses
4. Returns the handle value to the caller

The **usage count** on a kernel object tracks how many handle-table entries across all processes point to it. `CloseHandle` decrements the count; the kernel frees the object when it reaches zero.

### Kernel Object States: Signaled vs. Non-Signaled

Every kernel object carries a **signaled / non-signaled** boolean observed by `WaitForSingleObject` and `WaitForMultipleObjects`. Transitions are object-specific: a process becomes signaled on exit, a pipe or mailslot read end becomes signaled when data arrives, a mutex becomes signaled when released. This unified model lets any IPC primitive be waited on with the same API.

---

## HANDLE Inheritance

A parent process can pass inheritable handles to a child process at spawn time using `CreateProcess`:

```cpp
BOOL CreateProcessA(
    LPCSTR                lpApplicationName,
    LPSTR                 lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL                  bInheritHandles,      // <-- key parameter
    DWORD                 dwCreationFlags,
    LPVOID                lpEnvironment,
    LPCSTR                lpCurrentDirectory,
    LPSTARTUPINFOA        lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
);
```

When `bInheritHandles` is `TRUE`, the kernel copies every handle-table entry marked **inheritable** into the child's fresh handle table, with the same handle value and address. The kernel object's usage count is incremented for each copied entry.

```
 Parent Handle Table              Child Handle Table
| HANDLE | Addr   | Inherit? |   | HANDLE | Addr   | Inherit? |
|--------|--------|----------|   |--------|--------|----------|
|  127   | 0x1200 |    Y     |-->|  127   | 0x1200 |    Y     |
|  168   | 0x1680 |    N     |   |  256   | 0x2400 |    Y     |
|  256   | 0x2400 |    Y     |-->|        |        |          |
```

Only entries marked inheritable are copied; handle 168 above is excluded. Inheritance propagates recursively — if the child spawns its own child with `bInheritHandles = TRUE`, all inheritable entries in the child's table are copied again, and usage counts increment again.

**Practical use for IPC:** Create the IPC object (pipe read end, mailslot handle, etc.) in the parent with `SECURITY_ATTRIBUTES.bInheritHandle = TRUE`, then spawn the child. The child receives the same handle value pointing to the same kernel object, allowing direct communication without any out-of-band handle exchange.

### Pseudo-Handles and `DuplicateHandle`

`GetCurrentProcess()` returns a **pseudo-handle** — a special constant (typically `(HANDLE)-1`) that always refers to the calling process but is not registered in the handle table. It cannot be inherited or passed to another process.

When a process needs its real, inheritable handle — for example, to let a child reference the parent — use `DuplicateHandle`:

```cpp
BOOL DuplicateHandle(
    HANDLE   hSourceProcessHandle,  // Process that owns the original handle
    HANDLE   hSourceHandle,         // Handle to duplicate
    HANDLE   hTargetProcessHandle,  // Process to receive the duplicate
    LPHANDLE lpTargetHandle,        // Out: new handle value in target process
    DWORD    dwDesiredAccess,
    BOOL     bInheritHandle,
    DWORD    dwOptions
);
```

`DuplicateHandle` creates a new handle-table entry in the target process pointing to the same kernel object, increments its usage count, and optionally closes the source atomically via `DUPLICATE_CLOSE_SOURCE`.

---

## Mailslots

A **mailslot** is a one-directional, broadcast-capable IPC channel. The server creates the slot and reads from it; one or more clients open it by name and write to it.

```cpp
HANDLE CreateMailslot(
    LPCTSTR              lpName,              // \\.\mailslot\<name>
    DWORD                nMaxMessageSize,     // 0 = any size
    DWORD                lReadTimeout,        // ms; MAILSLOT_WAIT_FOREVER
    LPSECURITY_ATTRIBUTES lpSecurityAttributes
);
// Returns INVALID_HANDLE_VALUE on failure
```

Clients open the slot with `CreateFile` using the same UNC-style name (`\\server\mailslot\name` for cross-machine, `\\.\mailslot\name` for local). Mailslots support **one-to-many** delivery: multiple clients can all write to the same slot name, and a single server reads them in order.

Trade-offs:
- **Simple API** — no connection setup.
- **Unreliable** — based on datagrams; delivery is not guaranteed, especially cross-network.
- **Read-only server** — the creator can only read; clients can only write.
- **Message-oriented** — each `WriteFile` produces one discrete message.

---

## Anonymous and Named Pipes

### Anonymous Pipes

Anonymous pipes provide a **unidirectional, byte-stream** channel between a parent and child process. The parent creates both ends with `CreatePipe`, then passes the appropriate end to the child via handle inheritance.

```cpp
BOOL CreatePipe(
    PHANDLE hReadPipe,         // Read end
    PHANDLE hWritePipe,        // Write end
    LPSECURITY_ATTRIBUTES lpPipeAttributes,  // set bInheritHandle=TRUE
    DWORD   nSize              // 0 = default buffer size
);
```

The parent keeps one end and the child inherits the other. Writes block when the buffer is full; reads block when it is empty — built-in flow control. Anonymous pipes only work between related processes; there is no name for an unrelated process to connect to.

### Named Pipes

Named pipes extend the pipe model to **bidirectional**, **connection-oriented** channels accessible by name, including across machines.

```cpp
HANDLE CreateNamedPipe(
    LPCTSTR lpName,              // \\.\pipe\<name>
    DWORD   dwOpenMode,          // PIPE_ACCESS_DUPLEX, etc.
    DWORD   dwPipeMode,          // PIPE_TYPE_BYTE or PIPE_TYPE_MESSAGE
    DWORD   nMaxInstances,
    DWORD   nOutBufferSize,
    DWORD   nInBufferSize,
    DWORD   nDefaultTimeOut,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes
);
```

Clients connect with `CreateFile`; the server accepts with `ConnectNamedPipe`. Both byte-stream (`PIPE_TYPE_BYTE`) and message (`PIPE_TYPE_MESSAGE`) modes are supported, and overlapped I/O enables non-blocking operation.

---

## Memory-Mapped Files (Shared Memory)

The highest-throughput IPC mechanism is shared memory via **memory-mapped files**. Two processes map the same file-mapping object into their respective address spaces; reads and writes go directly to shared physical pages with no kernel-copy overhead.

```cpp
// Producer
HANDLE hMap = CreateFileMapping(
    INVALID_HANDLE_VALUE,  // page-file backed (no real file)
    NULL,
    PAGE_READWRITE,
    0, 4096,               // size
    L"Local\\MySharedMem"  // named so consumers can open it
);
LPVOID pBuf = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

// Consumer (separate process)
HANDLE hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"Local\\MySharedMem");
LPVOID pBuf = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
```

Both processes receive a pointer into the same mapped region. Synchronization (mutex, semaphore, event) must be layered on top — the mapping itself provides no ordering guarantees.

Trade-offs vs. pipes:
- **Zero-copy** for large data; pipes always copy data into and out of kernel buffers.
- **Requires explicit synchronization** — pipes have implicit flow control built in.
- **Persistent until all handles are closed** — lingering mappings can cause subtle bugs if one side exits without unmapping.

---

## Choosing a Mechanism

| Mechanism | Direction | Scope | Best for |
|-----------|-----------|-------|----------|
| Mailslot | One-way, broadcast | Local / network | Simple notifications, one-to-many |
| Anonymous pipe | One-way | Parent–child only | Redirecting stdio, simple child I/O |
| Named pipe | Bidirectional | Local / network | RPC-style client-server on same or remote machine |
| Memory-mapped file | Shared read/write | Local (with sync) | High-throughput bulk data exchange |

---

*Study notes based on 윤성우, 《윈도우즈 시스템 프로그래밍》 (book + lectures).*
