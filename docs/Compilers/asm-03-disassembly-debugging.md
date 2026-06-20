---
title: "Debugging Through Disassembly: Stack and Pointer Corruption"
---

# Debugging Through Disassembly: Stack and Pointer Corruption

Low-level memory corruption is one of the hardest classes of bug to diagnose at the source level. The symptoms — wrong variable values, access violations that appear in the wrong function, crashes that only reproduce in Release — only make sense once you can read the stack frame in the disassembly window. This post covers the workflow and the three concrete corruption scenarios you will encounter: uninitialized/dangling local variables, `ebp` contamination, and `eip` contamination.

> All examples use 32-bit x86 (MSVC Debug build) unless stated otherwise. Learning on 32-bit is recommended — the frame structure is more explicit than 64-bit.

---

## Setting Up the Disassembly Debugger (Visual Studio)

Open the disassembly view while stopped at a breakpoint:

- **Ctrl+Alt+D** — open Disassembly window
- Enable: *Show Code Bytes*, *Show Addresses*, *Show Source Code*, *Show Symbol Names*

Companion windows:

| Window | Shortcut | What to watch |
|---|---|---|
| Watch | Ctrl+Alt+W | Enter `ebp`, `esp`, `eax`, etc. — right-click → *Hexadecimal Display* |
| Call Stack | Ctrl+Alt+C | Right-click → *Show External Code* to see frames outside `main` |
| Memory | Ctrl+Alt+M, 1–4 | Set columns to 4 — shows 4 DWORDs (16 bytes) per row |
| Registers | Debug → Windows → Registers | Full `EAX EBX ECX EDX ESI EDI EIP ESP EBP EFL` view |

Watch expressions support format specifiers: `ebp,x` (hex), `ebp,d` (decimal), `ebp,b` (binary), `&a` (address of local).

---

## Reading the Function Prologue

Every 32-bit function opens with the same prologue. Recognizing it is the foundation of stack reading.

```asm
000719C0  55                   push        ebp
000719C1  8B EC                mov         ebp,esp
000719C3  81 EC D8 00 00 00    sub         esp,0D8h
000719C9  53                   push        ebx
000719CA  56                   push        esi
000719CB  57                   push        edi
```

What each line does:

1. **`push ebp`** — saves the *caller's* base pointer onto the stack. One byte opcode (`55`) because the Intel one-byte opcode map has dedicated encodings for common register pushes.
2. **`mov ebp, esp`** — establishes a new stack frame base. All local variables are now addressable as `[ebp - N]`.
3. **`sub esp, 0D8h`** — reserves 0xD8 = 216 bytes for local variables. The compiler allocates a fixed block up front.
4. **`push ebx/esi/edi`** — these are *callee-saved* registers (x86 calling convention). The function must restore them before returning.

After the prologue, MSVC Debug fills the reserved area with `0xCCCCCCCC` using `rep stos`:

```asm
8D 7D D8    lea    edi,[ebp-28h]     ; destination = first local variable slot
B9 0A 00 00 00  mov  ecx,0Ah         ; repeat 10 times
B8 CC CC CC CC  mov  eax,0CCCCCCCCh  ; fill value
F3 AB       rep stos dword ptr [edi] ; write eax to [edi], edi+=4, ecx-- until ecx==0
```

This is why uninitialized locals show `0xCCCCCCCC` in debug — it is a deliberate marker, not garbage.

---

## The Stack Frame Layout

Understanding what lives where is the prerequisite for every corruption scenario.

```
| Higher Address         |
| Caller stack frame     |
|------------------------|
| old ebp  (4 bytes)     |  <- push ebp saved this; pop ebp restores it
|------------------------|
| Canary   [ebp-4]       |  <- __security_cookie XOR'd with ebp
|------------------------|
| Local variables        |  <- [ebp-8], [ebp-12], ...
|------------------------|
| Saved ebx / esi / edi  |
| Lower Address          |
```

The return address (`eip` backup) lives *below* the saved `ebp`, just above the call parameters:

```
| ... caller frame ...   |
| eip backup (ret addr)  |  <- ret pops this into eip
| old ebp backup         |  <- pop ebp loads this
| Canary [ebp-4]         |
| Local variables        |
```

---

## Scenario 1 — Local Variable Contamination via Out-of-Bounds Pointer

The most common corruption path: a pointer to a local array walks past the array boundary and overwrites adjacent stack memory.

```cpp
static void func() {
    volatile int arr[4] = { 0, };
    volatile int *ptr;

    ptr = arr;
    ptr += 3;  *ptr = 0xABCDABCD;  // last element of arr — fine
    ptr += 1;  *ptr = 0xEEEEEEEE;  // Debug padding (0xCCCCCCCC slot)
    ptr += 1;  *ptr = 0xDDDDDDDD;  // Canary [ebp-4]
    ptr += 1;  *ptr = 0xCCCCCCCC;  // old ebp backup  <-- DANGER
    ptr += 1;  *ptr = 0xBBBBBBBB;  // eip backup      <-- CRITICAL
}
```

Memory view (Ctrl+Alt+M → 1) before the writes:

```
0x0055F7B8  00 00 00 00   arr[3]
0x0055F7BC  cc cc cc cc   Debug padding
0x0055F7C0  7a f8 a0 80   Canary (security_cookie XOR ebp)
0x0055F7C4  98 f8 55 00   old ebp
0x0055F7C8  94 19 5b 00   old eip (return address into caller)
```

After the writes, every slot from `arr[3]` upward is overwritten. The function itself still runs correctly — it accesses locals via the current `ebp` which has not moved yet. The damage surfaces when control returns to the caller.

**Debug vs. Release difference:** Debug builds insert a padding slot (`0xCCCCCCCC`) between the last local and the canary. Release builds do not. A write that only reaches the canary in Debug may reach `old ebp` in Release — a bug that is invisible in test builds.

---

## Scenario 2 — Base Pointer (`ebp`) Contamination

### What it looks like

A caller function crashes or produces wrong values on its local variables *after* a callee returns. Classic symptom:

```cpp
__declspec(noinline)
static void parent() {
    int a = 0;
    printf("%d", a);
    func1();
    a++;   // Access violation here — but func1 is the suspect
}
```

The read violation on `a` means `[ebp - N]` is resolving to unmapped memory. The `ebp` register has the wrong value.

### Why it happens

`func1` (or something it calls) walked its stack pointer past its own frame and overwrote the slot holding `parent`'s saved `ebp`. When `func1` executed `pop ebp`, the contaminated value loaded into `ebp`. From that point on, every `[ebp - N]` in `parent` references the wrong memory region.

The epilogue sequence makes this clear:

```asm
00893481  mov    esp,ebp
00893483  pop    ebp        ; <-- loads contaminated value into ebp
00893484  ret
```

### How to detect it

**With a local variable present** — assign a local to a `volatile static` before and after each suspected call, and watch for divergence:

```cpp
static void parent() {
    volatile static int g_check;
    int a = 1;
    func1();
    g_check = a;    // if g_check != 1, ebp is contaminated after func1
    func3();
    g_check = a;    // check again after func3
}
```

**Without a local variable** — use `_AddressOfReturnAddress()` to snapshot the saved `ebp` directly:

```cpp
#include <intrin.h>

__declspec(noinline)
static void parent() {
    volatile static uintptr_t* g_ebptr;
    volatile static uintptr_t  g_ebpvalue;

    // _AddressOfReturnAddress() points to eip backup; one slot below is ebp backup
    g_ebptr   = reinterpret_cast<uintptr_t*>(
                    (uintptr_t)_AddressOfReturnAddress() - sizeof(void*));
    g_ebpvalue = *g_ebptr;

    func1();

    g_ebptr = reinterpret_cast<uintptr_t*>(
                  (uintptr_t)_AddressOfReturnAddress() - sizeof(void*));
    if (g_ebpvalue != *g_ebptr) __debugbreak();  // ebp backup was overwritten
}
```

### The MSVC canary (`__security_cookie`)

MSVC inserts a canary at `[ebp-4]` in functions that use pointer arithmetic:

```asm
; Prologue
mov  eax, dword ptr [__security_cookie]
xor  eax, ebp
mov  dword ptr [ebp-4], eax          ; store (cookie XOR ebp)

; Epilogue
mov  ecx, dword ptr [ebp-4]
xor  ecx, ebp
call @__security_check_cookie@4      ; verifies ecx == __security_cookie
```

The check catches writes to `[ebp-4]`. It does **not** catch a write that skips the canary slot and hits `[ebp]` directly — which is exactly what the `ptr += 1` scenario above demonstrates after skipping past the debug padding.

---

## Scenario 3 — Instruction Pointer (`eip`) Contamination

### What `ret` actually does

```asm
mov  esp, ebp
pop  ebp        ; esp += 4, ebp = *esp
ret             ; eip = *esp, esp += 4
```

`ret` pops the return address off the stack into `eip`. If that slot was overwritten (one slot below the saved `ebp`), execution jumps to an arbitrary address. The program either crashes immediately at an unmapped address or — worse — executes attacker-controlled code.

### Two causes to distinguish

1. **Bad function pointer** — a `call` instruction received an invalid address (null, dangling, miscast function pointer).
2. **Stack return address overwrite** — the `eip` backup slot on the stack was overwritten by out-of-bounds pointer arithmetic or a buffer overrun.

### Tracing the caller when `eip` is corrupted

When `eip` is corrupted and the debugger is not showing a valid call stack, navigate the chain manually using the memory window:

```
| ebp (callee)  | <- register; points to this slot
|---------------|
| eip backup    | <- corrupted — but ebp is still intact
|---------------|
| Parameters    |
|---------------|
| ebp (caller)  | <- stored at address [ebp callee]
|---------------|
| eip backup    | <- valid return address into grandparent function
|---------------|
```

Procedure:

1. Read the current `ebp` register value in the Registers window.
2. The value stored at `[ebp]` (i.e., the memory at that address) is the **caller's `ebp`**.
3. The value stored at `[ebp] + 4` is the **caller's return address** — a valid address inside the `.text` section.
4. Look that address up in the Disassembly window to identify which function made the corrupting call.

This walk — current `ebp` → saved `ebp` → saved `eip` — gives you the parent function of whatever overwrote the return address.

---

## 64-bit Differences

In 64-bit Debug builds, `rbp` mirrors `ebp` and the frame structure is analogous. In 64-bit **Release** builds, the compiler may omit the frame pointer entirely, using pure `rsp`-relative addressing:

```asm
sub  rsp, 48h
; ... function body using [rsp+N] offsets ...
add  rsp, 48h
ret
```

Consequences:

- `rbp` contamination disrupting local variable access is **much less likely** in 64-bit Release.
- The call stack is harder to walk manually because there is no `rbp` chain.
- 64-bit has stronger hardware and OS-level mitigations (ASLR, shadow stack on newer CPUs).

For learning and deliberate corruption experiments, 32-bit Debug is the clearest environment.

---

## Quick Reference: What Each Corruption Looks Like

| Symptom | Register/slot to check | Likely cause |
|---|---|---|
| Wrong value in caller local after callee returns | `ebp` register | Saved `ebp` slot overwritten inside callee |
| Access violation on caller local after callee returns | `ebp` register | Same — `ebp` points to unmapped region |
| Crash at nonsense address on function return | `eip` at `ret` | Return address slot overwritten |
| Crash inside `__security_check_cookie` | Canary `[ebp-4]` | Pointer walk hit canary slot |
| Wrong behavior in Release but not Debug | — | Pointer walk hits `ebp`/`eip` slot that Debug padding shielded |

---

## Creating a Dump for Post-Mortem Analysis

When the crash is not reproducible under the debugger, capture a `.DMP` file:

- Task Manager → Details → right-click process → *Create Dump File* → saved to `%LOCALAPPDATA%\Temp\ProcessName.DMP`
- Open in Visual Studio or WinDbg; select **Native Only** debug mode.

The dump preserves register state and stack memory at the moment of crash — the same frame layout described above is visible even without a live process.
