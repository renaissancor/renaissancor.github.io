# Function & Calling Convention

## Functions and Stack Frames

- When a function is called, local variables and parameters are stored on the stack.
- A frame pointer (FP) is used to refer to the current stack frame.
- Stack frames are organized based on the stack pointer (SP).
- Function parameters are pushed onto the stack from right to left (C-style).
- The Pascal style (left to right) is rarely used in modern systems.

## Stack Operations

### Push Example

Storing the value 7 onto the stack:

```asm
ADD r1, 7, 0        ; r1 ← 7
STORE r1, [sp]      ; -sp ← r1
ADD sp, sp, 4       ; sp ← sp + 4
```

This is equivalent to:

```asm
push 7
```

### Pop Example

```asm
SUB sp, sp, 4       ; sp ← sp - 4
LOAD r1, [sp]       ; r1 ← -sp
```

This is equivalent to:

```asm
pop → r1
```

## Frame Pointer and Function Call

Entering a function:

```asm
PUSH bp             ; Save previous base pointer
MOV bp, sp          ; Set new base pointer
SUB sp, size        ; Allocate space for local variables
```

Exiting a function:

```asm
MOV sp, bp          ; Deallocate local variables
POP bp              ; Restore previous base pointer
RET                 ; Return to caller
```

## Program Counter and Link Register

- The Program Counter (PC) holds the address of the currently executing instruction.
- When a function call occurs, the current PC value must be saved to return later.
- This role is handled by the Link Register (LR).
- The Link Register functions similarly to the Base Pointer, storing the return address.

## Calling Conventions

### 32-bit Systems

| Convention   | Parameter Passing        | Stack Cleanup |
| ------------ | ------------------------ | ------------- |
| `__cdecl`    | Stack (right to left)    | Caller        |
| `__stdcall`  | Stack (right to left)    | Callee        |
| `__fastcall` | ecx, edx then stack      | Callee        |
| `__thiscall` | ecx (for `this` pointer) | Callee        |

### 64-bit Systems

#### Windows (MS ABI)

- Parameters: `rcx`, `rdx`, `r8`, `r9`
- Floating point: `xmm0` to `xmm3`
- Stack cleanup: Caller

#### Linux (System V ABI)

- Parameters: `rdi`, `rsi`, `rdx`, `rcx`, `r8`, `r9`
- Floating point: `xmm0` to `xmm7`
- Stack cleanup: Caller

## Summary

- Function parameters are pushed onto the stack from right to left.
- Upon function entry, the current base pointer is saved and a new stack frame is created.
- Upon function exit, the previous stack frame is restored using the base pointer.
- The Link Register saves the return address, enabling return to the caller.
- The stack typically grows downward (higher to lower addresses), so `SUB sp, size` allocates space.



