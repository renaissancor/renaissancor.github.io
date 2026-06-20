---
title: "Windows Processes and Kernel Objects"
---

# Windows Processes and Kernel Objects

Windows exposes process creation and management entirely through the kernel object model. Understanding that model — what a kernel object is, how HANDLEs work, and what usage counts govern — is the prerequisite for writing correct Win32 process code.

## Creating a Process: `CreateProcess`

On Windows, every new process is born through `CreateProcess`. Unlike POSIX `fork`/`exec`, there is no two-step: a single call creates the process, loads the executable, and spins up the primary thread.

```cpp
BOOL CreateProcess(
    LPCTSTR lpApplicationName,                  // 1. Executable path (or NULL)
    LPTSTR  lpCommandLine,                       // 2. Command line (argv[0] + args)
    LPSECURITY_ATTRIBUTES lpProcessAttributes,  // 3. Security descriptor for process object
    LPSECURITY_ATTRIBUTES lpThreadAttributes,   // 4. Security descriptor for thread object
    BOOL    bInheritHandles,                     // 5. Child inherits parent's inheritable handles
    DWORD   dwCreationFlags,                     // 6. e.g. CREATE_NEW_CONSOLE, priority flags
    LPVOID  lpEnvironment,                       // 7. Environment block (NULL = inherit parent)
    LPCTSTR lpCurrentDirectory,                  // 8. Starting directory (NULL = inherit parent)
    LPSTARTUPINFO lpStartupInfo,                 // 9. Window/console appearance
    LPPROCESS_INFORMATION lpProcessInformation   // 10. [out] HANDLEs and IDs for new process/thread
);
```

Either `lpApplicationName` or `lpCommandLine` can carry the executable name; passing `NULL` for the first and the full command in the second is the common idiom.

### `STARTUPINFO`

`STARTUPINFO` controls the visual presentation of the new process's window. Typical usage zero-initialises the struct and sets only `cb`:

```cpp
STARTUPINFO si;
ZeroMemory(&si, sizeof(si));
si.cb = sizeof(si);          // must be set; all other fields default to inherited
```

The struct also exposes `hStdInput`, `hStdOutput`, `hStdError` — useful when you want to redirect the child's standard I/O to pipes.

### `PROCESS_INFORMATION`

On success, `CreateProcess` fills a `PROCESS_INFORMATION` struct:

```cpp
typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;    // handle to the new process kernel object
    HANDLE hThread;     // handle to its primary thread kernel object
    DWORD  dwProcessId; // PID (system-wide unique number)
    DWORD  dwThreadId;  // TID
} PROCESS_INFORMATION;
```

Both handles must eventually be closed with `CloseHandle`; failing to do so is the classic kernel-object leak.

### Querying the working directory

```cpp
DWORD GetCurrentDirectory(
    DWORD  nBufferLength,  // buffer size in TCHARs
    LPTSTR lpBuffer        // receives the path
);
// Returns characters written, or 0 on failure (call GetLastError())
```

## Process Termination and Exit Codes

A process ends in one of two ways:

| Method | Notes |
|--------|-------|
| `return` from `main` / `WinMain` | Orderly shutdown — destructors run, buffers flush |
| `ExitProcess(UINT uExitCode)` | Immediate exit from anywhere in the process |
| `TerminateProcess(HANDLE, UINT)` | Forced kill from another process — **avoid** |

`TerminateProcess` is dangerous: it stops the target immediately without running C++ destructors, flushing file buffers, or releasing locks. Reserve it for true emergency situations. The exit code is stored in the process kernel object and retrieved later with `GetExitCodeProcess`.

## Kernel Objects

When Windows creates a process it also creates a **kernel object** — a kernel-managed data structure that tracks the resource's state. Kernel objects exist for many resource types:

- Process, Thread
- Mutex, Semaphore, Event
- File, Pipe, Socket
- Timer, Job Object, I/O Completion Port

Each kernel object stores the authoritative state for its resource. For a process that includes:

- **Execution state** — Running, Ready, Blocked
- **Priority class**
- **Exit code** (set when the process terminates)

Crucially, kernel objects are **owned by the OS kernel**, not by any individual process. They outlive the process that created them — which is why you can still read an exit code after the child has exited.

## The HANDLE Abstraction

You never touch a kernel object directly. The kernel exposes them through **HANDLEs**, and every process maintains its own private **handle table** that maps HANDLE values to kernel object pointers.

```
Process A                 Kernel
┌──────────────┐         ┌─────────────────────┐
│ Handle Table │         │ Process KO for B     │
│  0x34 ───────┼────────►│  state, priority...  │
└──────────────┘         └─────────────────────┘

Process B
┌──────────────┐
│ Handle Table │
│  0xA8 ───────┼────────► (same kernel object)
└──────────────┘
```

Key points:

- **Process-local values**: HANDLE `0x34` in process A and `0xA8` in process B can point to the same kernel object. Passing a raw HANDLE value across processes is meaningless.
- **Type-agnostic**: A single `HANDLE` type refers to any kernel object — process, thread, file, mutex, etc.
- **Handle inheritance**: When `bInheritHandles = TRUE` in `CreateProcess`, handles marked inheritable in the parent are copied into the child's handle table with the **same numeric value** — the one case where a HANDLE value is stable across a process boundary.

Manipulating a process kernel object through its HANDLE:

```cpp
// Child raises its own priority
SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

// Parent lowers the child's priority using the handle from PROCESS_INFORMATION
SetPriorityClass(pi.hProcess, NORMAL_PRIORITY_CLASS);
```

`GetCurrentProcess()` returns a special **pseudo-handle** that always refers to the calling process; it does not need to be closed.

## Usage Count and Object Lifetime

Because kernel objects outlive individual processes, they need their own lifetime management. Windows uses a **usage count** (reference count) on every kernel object.

| Event | Effect on usage count |
|-------|----------------------|
| Process created | Process KO created with UC = 1 |
| `CreateProcess` succeeds | Parent receives a HANDLE → UC becomes **2** |
| `CloseHandle(pi.hProcess)` | UC decremented by 1 |
| Process exits normally | UC decremented by 1 |
| UC reaches 0 | Kernel destroys the object |

This means a freshly spawned child process starts with UC = 2: one reference held by the child itself, one by the parent's HANDLE. If the parent closes its handle immediately after creation (`CloseHandle(pi.hProcess)`), UC drops to 1; when the child exits UC drops to 0 and the kernel object is destroyed.

**If the parent never calls `CloseHandle`**, the child's kernel object persists indefinitely — even after the child process has exited — because the parent still holds a reference:

```cpp
// WRONG: handle leak — child KO accumulates in memory
while (true) {
    ZeroMemory(&pi, sizeof(pi));
    CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
    // CloseHandle(pi.hProcess);  // <-- omitted!
    // CloseHandle(pi.hThread);
}

// CORRECT: release the handle once you no longer need it
CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
// ... use pi.hProcess if needed (e.g. WaitForSingleObject) ...
CloseHandle(pi.hProcess);
CloseHandle(pi.hThread);
```

The rule of thumb: call `CloseHandle` on every HANDLE you receive. If you need to query the child's exit code later, keep the handle open until after `GetExitCodeProcess`, then close it.

## Putting It Together: Minimal Parent/Child Example

```cpp
// parent.cpp
#include <windows.h>
#include <cstdio>

int main() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    BOOL ok = CreateProcess(
        NULL,                   // use command line for exe name
        L"child.exe",           // command line
        NULL, NULL,
        FALSE,                  // don't inherit handles
        0,                      // default creation flags
        NULL, NULL,             // inherit environment and cwd
        &si, &pi
    );

    if (!ok) {
        printf("CreateProcess failed: %lu\n", GetLastError());
        return 1;
    }

    // Wait for child to finish, then retrieve exit code
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    printf("Child exited with code: %lu\n", exitCode);

    // Release kernel object references
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}
```

## Summary

| Concept | Key point |
|---------|-----------|
| `CreateProcess` | Single call; fills `PROCESS_INFORMATION` with two HANDLEs |
| Kernel object | OS-owned data structure; holds authoritative resource state |
| HANDLE | Process-local index into a per-process handle table |
| Usage count | Starts at 2 for a child process; `CloseHandle` decrements it |
| `CloseHandle` | Releases the HANDLE reference — **does not terminate the process** |
| `TerminateProcess` | Forces termination — avoid; prefer letting the process `return` from `main` |

---

*Study notes based on 윤성우, 《윈도우즈 시스템 프로그래밍》 (book + lectures).*
