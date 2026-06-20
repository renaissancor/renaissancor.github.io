---
title: "Stack Frames, Calling Conventions, and Memory Layout"
---

# Stack Frames, Calling Conventions, and Memory Layout

How the machine-level ABI and stack work, as seen through x86/x64 disassembly.

---

## Process Memory Layout

A running process on Windows is laid out from low to high address as follows (stack base assumed at `0x23010000`, stack size 1 MB = `0x00010000`):

```text
|--------------|  Low Address  (0x23000000)
| .text (CODE) |  Machine code (instructions)
|--------------|
| .rodata      |  Read-only data (const literals, string literals)
|--------------|
| .data        |  Initialized global/static variables
|--------------|
| .bss         |  Uninitialized global/static variables (zero-filled at startup)
|--------------|
|  heap        |  Dynamic allocation — grows upward (malloc, new)
|~~~~~~~~~~~~~~|
|    (gap)     |  Unused address space
|~~~~~~~~~~~~~~|
|  stack       |  Function call stack — grows downward (esp = esp - frame_size)
|--------------|  High Address (0x23010000)
```

Key takeaways:

- **Stack grows downward** — allocating a frame decrements `esp`/`rsp`.
- **Heap grows upward** — they meet in the middle; OS enforces a guard page.
- `.bss` is zero-initialized before `main()` runs, but the space is not stored in the executable file — only its size is recorded.

### Segment example

```cpp
const char* msg = "hello";    // *msg lives in .rodata; the pointer itself is on the stack/data
int global_arr[1000] = {0,};  // .data — inflates the .exe by 4000 bytes
int uninit_arr[2000];         // .bss  — 8000 bytes allocated at startup, not stored in .exe
```

### .rodata vs stack const — a disassembly gotcha

```cpp
const int g_x = 1000;   // .rodata — write-protected page

int main() {
    const int y = 2000;                      // stack — read/write
    int* p_y = const_cast<int*>(&y);
    *p_y = 3000;                             // succeeds (stack is r/w)
    int* p_x = const_cast<int*>(&g_x);
    *p_x = 4000;                             // RUNTIME ERROR — .rodata page fault
}
```

The compiler goes further: for a local `const int a = 100`, it often **folds the constant inline** rather than reading it from the stack address. This means that even after writing through a pointer, the original `printf("%d", a)` call still prints `100`:

```asm
; const int a = 100;
006B49B0  mov  dword ptr [ebp-0Ch], 64h   ; store 100 on stack

; printf("%d", a);          <- compiler uses literal 64h, NOT a memory read
006B49B7  push 64h
006B49B9  push 6B7BD0h                    ; address of "%d\n"
006B49BE  call 006B13B1                   ; printf

; *p = 999;                 <- writes to stack address, but never read back for 'a'
006B49CC  mov  eax, dword ptr [ebp-18h]
006B49CF  mov  dword ptr [eax], 3E7h
```

The `volatile` keyword prevents this optimization: it forces every access to go through memory.

---

## Stack Frame Anatomy

### Prologue and epilogue (x86 debug mode)

Every non-inlined function opens with a **prologue** that saves the caller's base pointer and reserves space for locals:

```asm
push ebp           ; save caller's frame base
mov  ebp, esp      ; establish new frame base
sub  esp, 48h      ; reserve 72 bytes for locals
push ebx           ; save callee-saved registers
push esi
push edi
```

And closes with an **epilogue** that unwinds:

```asm
pop  edi
pop  esi
pop  ebx
mov  esp, ebp      ; restore stack pointer
pop  ebp           ; restore caller's frame base
ret
```

`ebp` (x86) / `rbp` (x64 debug) acts as a stable anchor: local variables are at negative offsets (`[ebp-4]`, `[ebp-8]`, …) and parameters from the caller are at positive offsets (`[ebp+8]`, `[ebp+C]`, …). The slot at `[ebp+4]` holds the **saved return address** pushed by `call`.

### x64 difference: rbp-less release frames

In x64 **debug** mode the `rbp`/`rsp` pair mirrors x86. In x64 **release** mode the compiler drops `rbp` entirely and tracks everything relative to `rsp`:

```
; No push rbp / mov rbp,rsp in release mode
sub  rsp, 28h      ; shadow space + locals, purely rsp-relative
```

This means a buffer overflow that corrupts `rbp` in a 32-bit debug build is silently harmless (the corrupted value just gets `pop`-ed back to `ebp`), but the `rip` (return address) slot is equally at risk in both modes. In 64-bit, frame analysis becomes harder because there is no stable base pointer to anchor the debugger.

### Caller/callee parameter layout on the stack (x86 cdecl)

```
High address
+--------------------+
| arg N              |  [ebp + 4+4*N]
| ...                |
| arg 1              |  [ebp+0Ch]
| arg 0              |  [ebp+08h]
+--------------------+
| return address     |  [ebp+04h]   <-- pushed by CALL
+--------------------+
| saved ebp          |  [ebp+00h]   <-- pushed by prologue
+--------------------+
| local var 0        |  [ebp-04h]
| local var 1        |  [ebp-08h]
| ...                |
Low address (stack grows this way)
```

---

## Calling Conventions: Struct Parameters

Passing a struct by value is legal C++ but has significant hidden cost at the ABI level.

### Small struct — fits in registers

```cpp
struct DATA { volatile int a; volatile int b; volatile char c; };
// sizeof == 12 (4+4+1+3 padding)
```

For a small struct, the compiler passes each member as a separate push and returns the two integer fields in `eax:edx`:

```asm
; DATA data2 = fdata(data1);
mov  eax, dword ptr [ebp-4]   ; data1.b
push eax
mov  ecx, dword ptr [data1]   ; data1.a
push ecx
call fdata
add  esp, 8                    ; cdecl caller cleans args
mov  dword ptr [ebp-58h], eax  ; return value low word
mov  dword ptr [ebp-54h], edx  ; return value high word
```

Inside `fdata`, the struct is accessed via positive `ebp` offsets (parameters from the caller's frame):

```asm
mov  eax, dword ptr [ebp+08h]  ; in.a
mov  dword ptr [data], eax
mov  eax, dword ptr [ebp+0Ch]  ; in.b
```

### Large struct — hidden return pointer + `rep movs`

When the struct contains a large array (e.g. `char c[47]`, total 56 bytes), the ABI switches to a **hidden output pointer** convention:

1. The caller allocates a return buffer on its own stack.
2. The address of that buffer is pushed as a hidden first argument.
3. The callee writes its return value directly into that buffer via `rep movs`.
4. The callee returns the buffer address in `eax`; the caller then copies from the buffer to the final destination.

```asm
; caller side (main)
sub  esp, 38h              ; reserve 56-byte copy of data1 for 'in' param
mov  ecx, 0Eh              ; 14 dwords = 56 bytes
lea  esi, [data1]
mov  edi, esp
rep movs dword ptr es:[edi], dword ptr [esi]   ; copy data1 -> arg slot

lea  eax, [ebp-124h]       ; address of return buffer
push eax                   ; hidden first argument
call fdata
add  esp, 3Ch              ; clean up (hidden ptr + 56-byte in-param)

; callee side (fdata) — return path
mov  ecx, 0Eh
lea  esi, [data]           ; local 'data' struct
mov  edi, dword ptr [ebp+8]; hidden pointer to caller's return buffer
rep movs dword ptr es:[edi], dword ptr [esi]
mov  eax, dword ptr [ebp+8]; return the buffer pointer
```

The resulting stack layout during `fdata`:

```text
High address
+-------------------------------+
|  Copy of data1 (56 bytes)     |  argument 'in'
+-------------------------------+
|  Address of return buffer     |  hidden first arg
+-------------------------------+
|  Return address               |
+-------------------------------+  <- fdata's EBP (after prologue)
|  Saved EBP (main's)           |
+-------------------------------+
|  Saved EBX, ESI, EDI          |
+-------------------------------+
|  fdata locals (incl. 'data')  |
+-------------------------------+
Low address
```

**Implication**: passing or returning a large struct by value can involve three separate `rep movs` copies (local → return buffer → temp in caller → final destination). Use a pointer or reference parameter to avoid this.

---

## Struct Alignment and Padding

All data types must start at an address that is a multiple of their size. The overall struct alignment equals the size of its largest member.

```cpp
struct DATA {
    char  a;    // 1 byte, offset 0
    char  b;    // 1 byte, offset 1
    short c;    // 2 bytes, offset 2 (naturally aligned)
    int   d;    // 4 bytes, offset 4 (naturally aligned)
    short e;    // 2 bytes, offset 8
    short f;    // 2 bytes, offset 10
    char  g;    // 1 byte,  offset 12
    // 3 bytes padding to reach multiple-of-4 boundary
};              // sizeof == 16
```

Memory map:

```
|a|b| c |          offset 0–3
|   d   |          offset 4–7
| e | f |          offset 8–11
|g|_|_|_|          offset 12–15  (3 bytes padding)
```

Rules apply universally across MSVC, GCC, and Clang on Windows, Linux, and macOS.

### Why alignment matters

- **Performance**: an unaligned read (e.g., `int* p = (int*)(buffer + 3)`) forces the CPU to issue two bus transactions to span the cache-line boundary.
- **Atomicity**: in a multithreaded context, an unaligned 4-byte value that straddles two cache lines can be torn — the two halves may be read at different times, observing a half-written value.
- **Cache lines**: RAM ↔ cache transfers happen in 64-byte chunks. A struct that is poorly padded may touch two cache lines where one would suffice.

### Dynamic allocation alignment

`malloc` guarantees 8-byte alignment on 32-bit platforms and 16-byte alignment on 64-bit platforms. Allocating even a single `char` via `malloc` on a 64-bit system wastes 15 bytes of padding. For SIMD types (`__m128`, `__m256`) that require 16- or 32-byte alignment, use `_aligned_malloc`.

### Struct member ordering tip

Member order is fixed at compile time and cannot be reordered by the optimizer. Declaring a struct with larger members first minimizes internal padding:

```cpp
// 16 bytes (wasteful ordering)
struct Bad  { char a; int b; char c; };   // a + 3pad + b + c + 3pad

// 12 bytes (better ordering)
struct Good { int b; char a; char c; };   // b + a + c + 2pad
```

---

## Context Switching at the Frame Level

A context switch is performed by the OS kernel: it saves the entire CPU register state (including `rsp`, `rip`, and all general-purpose registers) for the outgoing thread and loads the saved state for the incoming thread. The call stacks themselves remain on their respective thread stacks in memory and are untouched.

**Cache effects**: the switch itself does not flush the L1/L2 cache, but loading a different thread's working set will evict the previous thread's cache lines through normal replacement. Some security-focused or SMT microarchitectures may partially partition the L1 cache, but this is not the default. The performance cost of a context switch therefore comes primarily from **cache cold-start** on the new thread, not from any explicit cache invalidation.

---

## Compiler Debug Configuration Notes

When analyzing disassembly, the build configuration matters:

| Setting | Debug default | Optimized-debug |
|---------|--------------|-----------------|
| Optimization | `/Od` (disabled) | `/O2` or `/Ox` |
| Runtime checks | `/RTC1` (stack frame + uninit vars) | Default (none) |
| Debug info format | `/ZI` (Edit & Continue PDB) | `/Zi` or `/Z7` |

`/RTCs` specifically inserts stack frame validation code; it catches stack corruption in debug builds that release builds silently allow through.

For x64 disassembly in Visual Studio: VS reads machine code backward from the breakpoint to reconstruct assembly for CISC variable-length instructions. Instructions *before* the breakpoint may be decoded incorrectly (you may see artifacts like `in al, dx`). Instructions *after* the breakpoint are reliable.
