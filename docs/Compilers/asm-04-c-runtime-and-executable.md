---
title: "The C Runtime and the Executable"
---

# The C Runtime and the Executable

Every C or C++ program contains code that runs before — and after — `main`. That code is the C Runtime Library (CRT). Understanding what it does, how it ends up in your binary, and how the OS loader brings that binary into memory makes the gap between "source file" and "running process" fully transparent.

---

## What the CRT Does Before `main`

When the OS transfers control to a new process it does not jump directly to `main`. It jumps to the **entry point** declared in the PE header — for MSVC-compiled programs that is `mainCRTStartup` (console subsystem) or `WinMainCRTStartup` (GUI subsystem).

`mainCRTStartup` performs several mandatory setup steps:

1. **Heap initialization** — calls `HeapCreate` / initializes the CRT heap so `malloc`/`new` work.
2. **Global and static constructors** — iterates the `.CRT$XI` initializer table and calls every C++ constructor for objects with static storage duration. This is why a global `std::string` is fully constructed by the time `main` starts.
3. **`argc`/`argv` parsing** — retrieves the command-line string from the OS and splits it into the familiar `argv` array.
4. **Environment block** — sets up `_environ` / `environ` from the process environment block.
5. **Calls `main`** — passes the prepared `argc`, `argv`, `envp`.
6. **Calls `exit`** — when `main` returns, `mainCRTStartup` passes the return value to `exit`, which flushes stdio buffers, runs `atexit` callbacks, and calls `ExitProcess`.

```cpp
// Simplified mental model of mainCRTStartup
int mainCRTStartup() {
    __heap_init();
    __initenv();          // environment block
    __cinit();            // global/static C++ constructors (.CRT$XI)
    int argc; char** argv;
    __getmainargs(&argc, &argv, ...);
    int ret = main(argc, argv);
    exit(ret);            // atexit callbacks, stdio flush, ExitProcess
}
```

### `atexit` and Cleanup

`atexit` registers a function to be called when `exit` runs. The CRT maintains an internal list; calls are executed in LIFO order. C++ destructors for static objects are also queued here via `__cxa_atexit` / `atexit` internally. Calling `ExitProcess` directly bypasses all of this.

---

## Static vs. Dynamic CRT Linking

Visual Studio exposes the choice through the **Runtime Library** project setting (`Project → Properties → C/C++ → Code Generation`).

| Flag | Mode | Effect |
|---|---|---|
| `/MT` | Multi-threaded static | CRT code compiled into `.exe` |
| `/MTd` | Multi-threaded static (Debug) | Same, with debug CRT |
| `/MD` | Multi-threaded DLL | Links against `vcruntime*.dll` + `ucrtbase.dll` |
| `/MDd` | Multi-threaded DLL (Debug) | Links against `ucrtbased.dll` |

**Static linking (`/MT`):** the linker copies the CRT object files from `libcmt.lib` directly into your `.exe`. A trivial console program that uses `printf` grows from ~48 KB to ~1.3 MB because the entire CRT is embedded. The resulting binary runs on any Windows machine without installing anything.

**Dynamic linking (`/MD`):** the `.exe` records an import dependency on `ucrtbase.dll` (the Universal CRT, shipped with Windows 10+) and `vcruntime140.dll`. These DLLs are loaded by the OS loader at process startup. The binary stays small, and if Microsoft ships a security fix to `ucrtbase.dll` your program benefits automatically. The trade-off: the DLL must be present. For redistribution, Microsoft provides the **Visual C++ Redistributable** package that installs the required DLLs.

**DLL memory sharing:** when multiple processes use `/MD` against the same `ucrtbase.dll`, the OS maps the DLL's read-only code pages into each process's virtual address space from a single physical copy — genuine RAM saving at scale.

> Mixing `/MT` and `/MD` across compilation units in the same binary is undefined behavior. The two runtimes have separate heaps; a pointer `malloc`-ed in one cannot be `free`-d in the other.

---

## The PE Executable Structure

A Windows executable is a **Portable Executable (PE)** file. Its layout on disk maps directly to what the loader pulls into memory.

```
PE File Layout
─────────────────────────────────────
DOS stub         (legacy MZ header)
PE signature     ("PE\0\0")
COFF header      (machine, section count, timestamp)
Optional header  (ImageBase, entry point RVA, subsystem)
Section table    (name, VirtualAddress, SizeOfRawData, ...)
─────────────────────────────────────
.text            executable code
.rdata           read-only data, import/export tables, string literals
.data            initialized global/static variables (read-write)
.bss             zero-initialized globals (no disk space, only VA range)
.CRT$XI          global constructor function pointers (MSVC)
.rsrc            resources (icons, manifests)
.reloc           base relocation table
─────────────────────────────────────
```

The **Optional Header** contains the `AddressOfEntryPoint` field — the RVA of `mainCRTStartup`. This is what the loader reads to know where to start execution.

The **Import Directory** (inside `.rdata`) lists every DLL the binary requires and, within each DLL, every function the binary calls. The loader resolves these at startup by filling in the **Import Address Table (IAT)**.

---

## How the Loader Brings the Executable to Life

1. **`CreateProcess` call** — the parent process asks the kernel to create a new process from a PE path.
2. **Map the image** — the kernel memory-maps the PE file at `ImageBase` (typically `0x00400000` for 32-bit, `0x140000000` for 64-bit). Sections become virtual memory regions with the permissions specified in the section table (`.text` → `PAGE_EXECUTE_READ`, `.data` → `PAGE_READWRITE`).
3. **Load dependencies** — the loader reads the Import Directory, recursively loads each required DLL into the process address space, and patches the IAT with the actual function addresses.
4. **Apply relocations** — if the image could not be mapped at its preferred `ImageBase` (ASLR or address conflict), the loader applies the `.reloc` table to fix every absolute address embedded in the code.
5. **TLS callbacks** — Thread-Local Storage callbacks run before the entry point (used by some anti-cheat and DRM systems).
6. **Transfer control** — the loader calls `mainCRTStartup` at the resolved entry point RVA. The runtime completes its setup and calls `main`.

---

## Practical Implications

**Executable patching and the entry point:** Because the entry point is an RVA in the Optional Header, a hex editor can redirect execution before `main` is ever reached. This is the mechanism older viruses and cracks used — they would append a payload to the end of the `.text` section and rewrite the entry point RVA to point to it, then jump back to `mainCRTStartup` after the payload ran. Modern Windows mitigates this with code signing and Authenticode, but the structural vulnerability is inherent to PE.

**Why static initialization order matters:** `mainCRTStartup` calls constructors in `.CRT$XI` in link order. If global object A depends on global object B, and B's translation unit is linked after A's, B is not yet constructed when A's constructor runs. This is the **Static Initialization Order Fiasco** — avoidable by using function-local statics (constructed on first call) or by keeping global objects independent.

**Debug vs. Release CRT:** `/MTd`/`/MDd` link the debug CRT, which fills uninitialized heap allocations with `0xCD`, freed memory with `0xDD`, and stack guards with `0xCC`. These sentinel bytes are the reason "my Release build crashes but Debug doesn't" — the debug CRT was masking an uninitialized read.

---

The CRT is not boilerplate — it is the bridge between a flat executable image on disk and a live process with a heap, constructed objects, and a stdio environment. Reading its startup sequence once makes the rest of the toolchain legible.
