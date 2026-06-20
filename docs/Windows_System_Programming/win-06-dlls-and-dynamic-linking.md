---
title: Windows DLLs and Dynamic Linking
---

# Windows DLLs and Dynamic Linking

A **Dynamic-Link Library (DLL)** is a binary file that contains executable code and data which multiple programs can load and share simultaneously. Understanding DLLs requires understanding the two fundamental ways a program can incorporate external code: static linking and dynamic linking.

## Static vs Dynamic Linking

When you compile a C/C++ project in Visual Studio, the **runtime library** setting controls how the C runtime code reaches the final executable.

**Static linking (`/MT`)** embeds the runtime library code directly into the `.exe`. Every function from the C runtime — `printf`, `malloc`, string utilities — is copied into your binary at link time. The result is a self-contained executable: it runs on any Windows machine without needing a separate DLL present. The trade-off is binary size; multiple programs that all link statically each carry their own copy of the same runtime code in memory.

**Dynamic linking (`/MD`)** leaves the runtime code in a separate DLL file. The `.exe` records *which* functions it needs and *which* DLL provides them, but the actual machine code lives elsewhere. At load time Windows resolves those references. The executable is smaller and, when multiple processes run, they share a single in-memory copy of the DLL. The trade-off is a deployment dependency: if the required DLL is absent or the wrong version, the process fails to start.

## Why DLLs Exist

Three core reasons drive the DLL model:

1. **Memory sharing.** When `kernel32.dll` is loaded by the first process, subsequent processes map the same physical pages into their address spaces. Functions like `GetTickCount` and `Sleep` exist once in RAM regardless of how many programs call them.
2. **Modularity and updates.** A DLL can be replaced without recompiling every application that uses it. Operating system updates ship as DLL replacements; applications pick up fixes automatically on next load.
3. **Separation of concerns.** Functionality can be split across independently-versioned binaries. A plugin architecture, for instance, is a natural fit for DLLs.

The system DLLs (`kernel32.dll`, `user32.dll`, `ntdll.dll`) ship with Windows and cover the entire Win32 API surface. Code that calls `Sleep` or `CreateFile` links against these implicitly; no extra linker step is required for standard Win32 functions.

## Exporting and Importing Symbols: `__declspec`

For a function or variable to be usable across a DLL boundary, the compiler needs to know its direction:

```cpp
// Inside the DLL project — mark this symbol for export
__declspec(dllexport) int Add(int a, int b) {
    return a + b;
}
```

```cpp
// Inside the consuming project — mark this symbol as coming from a DLL
__declspec(dllimport) int Add(int a, int b);
```

The conventional pattern is a single header that toggles between the two based on whether it is included from inside the DLL or from a client:

```cpp
#ifdef MYLIB_EXPORTS
    #define MYLIB_API __declspec(dllexport)
#else
    #define MYLIB_API __declspec(dllimport)
#endif

MYLIB_API int Add(int a, int b);
```

When building the DLL project, define `MYLIB_EXPORTS`. Client projects omit it and get `dllimport` automatically.

## The Import Library: `.lib` vs `.dll`

Building a DLL produces two output files:

- **`MyLib.dll`** — the actual DLL loaded at runtime. It contains the machine code.
- **`MyLib.lib`** — the *import library*. It contains no real code; it is a stub table that tells the linker which symbols live in which DLL and what their decorated names are.

Client projects link against `MyLib.lib` at build time. The linker records the dependency. At runtime, Windows loads `MyLib.dll` and resolves the calls. The `.lib` file is a build-time artifact only; it is not needed at runtime.

This distinction matters in practice: distributing a DLL to end users requires only the `.dll`. The `.lib` (and header files) belong in a developer SDK package.

## Specifying Libraries in Source: `#pragma comment`

Rather than configuring linker input through project properties, you can embed the dependency directly in source code:

```cpp
#pragma comment(lib, "winmm.lib")
```

This is exactly equivalent to adding `winmm.lib` to **Project Properties → Linker → Input → Additional Dependencies**. It is convenient for self-contained code samples and headers that know their own link requirements. The Windows Multimedia API (`PlaySound`, timer functions) uses `winmm.lib` and is a common example.

## Implicit vs Explicit Linking

### Implicit (Load-Time) Linking

The standard approach. The client links against the import library; Windows loads the DLL before `main` executes and resolves all imports. If the DLL is missing, the process fails to start immediately with a system error.

```cpp
// Nothing special needed — just include the header and link the .lib
#include "MyLib.h"
#pragma comment(lib, "MyLib.lib")

int main() {
    int result = Add(3, 4);  // resolved at load time
}
```

### Explicit (Run-Time) Linking

The program loads the DLL itself using the Win32 API `LoadLibrary` and retrieves function pointers with `GetProcAddress`. The DLL is not required at startup; the program can test whether it is present and handle its absence gracefully.

```cpp
#include <windows.h>

typedef int (*AddFunc)(int, int);

int main() {
    HMODULE hLib = LoadLibrary(TEXT("MyLib.dll"));
    if (hLib == NULL) {
        // DLL not found — handle gracefully
        return 1;
    }

    AddFunc Add = (AddFunc)GetProcAddress(hLib, "Add");
    if (Add != NULL) {
        int result = Add(3, 4);
    }

    FreeLibrary(hLib);
    return 0;
}
```

`LoadLibrary` returns an `HMODULE` (essentially an `HINSTANCE`), which is the base address where the DLL was mapped. `GetProcAddress` takes that handle and a symbol name string, returning a `void*` that you cast to the appropriate function pointer type. `FreeLibrary` decrements the DLL's reference count; when it reaches zero, Windows unloads it.

Explicit linking is the mechanism behind plugin systems, optional feature packs, and version-probing code (check whether a newer API exists before calling it).

## DllMain: The DLL Entry Point

Every DLL can optionally define `DllMain`, which Windows calls at four points in the DLL's lifecycle:

```cpp
BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            // DLL loaded into a process — initialize resources
            break;
        case DLL_PROCESS_DETACH:
            // DLL unloaded from a process — release resources
            break;
        case DLL_THREAD_ATTACH:
            // A new thread was created in the hosting process
            break;
        case DLL_THREAD_DETACH:
            // A thread in the hosting process exited
            break;
    }
    return TRUE;  // FALSE on PROCESS_ATTACH aborts the load
}
```

`DLL_PROCESS_ATTACH` fires once when the DLL is first mapped — whether via implicit load at startup or via `LoadLibrary`. It is the correct place to allocate per-process DLL resources. `DLL_PROCESS_DETACH` fires when the DLL is unmapped, either because `FreeLibrary` dropped the reference count to zero or the process is exiting.

A critical constraint: `DllMain` runs under the loader lock. Calling `LoadLibrary`, most synchronization primitives, or anything that might trigger another DLL load from within `DllMain` risks deadlock. Keep `DllMain` minimal.

## Summary

| Concept | Key point |
|---|---|
| Static linking (`/MT`) | Runtime code copied into `.exe`; no DLL dependency |
| Dynamic linking (`/MD`) | Runtime code stays in DLL; shared in memory across processes |
| `__declspec(dllexport)` | Marks a symbol for export from a DLL |
| `__declspec(dllimport)` | Marks a symbol as imported from a DLL |
| Import library (`.lib`) | Build-time stub; tells linker what is in which DLL |
| `#pragma comment(lib, …)` | Embeds linker input directive in source |
| Implicit linking | DLL loaded before `main`; failure = process abort |
| Explicit linking | `LoadLibrary` / `GetProcAddress` at runtime; optional DLL |
| `DllMain` | DLL entry point for attach/detach notifications |

---

*Study notes based on 윤성우, 《윈도우즈 시스템 프로그래밍》 (book + lectures).*
