---
title: "C++ Fundamentals — Types, Operators, and Control Flow"
---

# C++ Fundamentals — Types, Operators, and Control Flow

## Data Types

C++ types determine memory layout and valid operations. The core integer ladder:

| Type | Size | Signed range | Unsigned range |
|------|------|-------------|----------------|
| `char` | 1 B | −128 to 127 | 0 to 255 |
| `short` | 2 B | −32 768 to 32 767 | 0 to 65 535 |
| `int` | 4 B | ±2 billion | 0 to ~4 billion |
| `long long` | 8 B | ±9.2 × 10¹⁸ | — |

Floating-point: `float` (4 B, ~7 sig. digits) and `double` (8 B, ~15 sig. digits), both IEEE 754.

**Overflow and wrap-around** — signed overflow is undefined behavior; unsigned overflow wraps modulo 2ⁿ:

```cpp
signed char sc = 128;      // UB — don't do this
unsigned char uc = -1;     // well-defined: wraps to 255
int i = 3.99;              // truncates to 3
```

**Implicit vs explicit conversion** — the compiler widens silently (int → float); use a C-style cast for intentional narrowing:

```cpp
float result = (float)iData + fData;
```

---

## Operators & Precedence

### Arithmetic

Integer division truncates; `%` is integers only:

```cpp
int a = 10, b = 3;
a / b   // 3   (truncated)
a % b   // 1
10.f / 3.f  // 3.333… (float division)
```

### Increment / Decrement

`++i` increments *before* the value is read; `i++` increments *after*:

```cpp
int i = 10;
int j = i++ * 2 + ++i;
// i++ yields 10 (i → 11); ++i yields 12 (i → 12); j = 10*2+12 = 32
```

Mixing these in a single expression is legal but hard to reason about — prefer separate statements.

### Logical operators and short-circuit evaluation

`&&` stops at the first `false`; `||` stops at the first `true`. Only `0` is false; every other integer is true.

### Precedence (high → low, abbreviated)

`!` / `~` / `++`/`--` (unary) → `*` `/` `%` → `+` `-` → `<<` `>>` → `<` `<=` `>` `>=` → `==` `!=` → `&` → `^` → `|` → `&&` → `||` → `=` (assignment)

When in doubt, add parentheses.

---

## Control Flow

### if / else if / else

Only the first matching branch executes; subsequent conditions are not tested:

```cpp
if (hp > 80)       { /* full */ }
else if (hp > 60)  { /* good */ }
else if (hp > 20)  { /* low  */ }
else               { /* critical */ }
```

Use separate `if` statements when every condition must be evaluated independently.

### switch / case

Useful for dense integer or enum branching. Always add `break` (or `[[fallthrough]]`) to avoid unintended fall-through.

### Loops

```cpp
// while
while (count < 4) { ++count; }

// for — preferred when the iteration variable is local to the loop
for (int i = 0; i < n; ++i) { /* … */ }

// break exits immediately; continue skips to next iteration
while (true) {
    if (done) break;
    if (skip) continue;
}
```

---

## Arrays and Scope

Arrays occupy contiguous memory; the array name decays to a pointer to the first element:

```cpp
int arr[10] = {1, 2};          // remaining elements zero-initialized
int count = sizeof(arr) / sizeof(arr[0]);  // element count
```

Use `sizeof(arr) / sizeof(arr[0])` rather than hardcoding the length.

**Variable shadowing** — redeclaring a name in an inner block hides the outer variable for the lifetime of that block:

```cpp
int a = 0;
{
    int a = 10;  // hides outer a
}
// outer a is 0 again here
```

---

## Bitwise Operators & Preprocessor Macros

Bitwise ops work at the individual-bit level. Always use `unsigned` types to avoid sign-bit surprises during shifts.

| Operator | Effect |
|----------|--------|
| `a & b` | AND — keep bits set in both |
| `a \| b` | OR — set bits set in either |
| `a ^ b` | XOR — toggle bits that differ |
| `~a` | NOT — invert all bits |
| `a << n` | left shift (× 2ⁿ) |
| `a >> n` | right shift (÷ 2ⁿ, use `unsigned`) |

### Bit flags pattern

Pack multiple boolean states into one integer using `#define` constants, each a unique power of two:

```cpp
#define ATT_UP   0x01
#define DEF_UP   0x02
#define SPEED_UP 0x08
#define EVENT_POTION (ATT_UP | DEF_UP | SPEED_UP)

unsigned long long stat = 0;
stat |= ATT_UP;              // set flag
stat |= EVENT_POTION;        // set multiple

if (stat & DEF_UP) { /* active */ }   // test flag
stat &= ~SPEED_UP;                     // clear flag
stat ^= ATT_UP;                        // toggle flag
```

Prefer `const` or `enum class` over `#define` when type safety matters; `#define` is appropriate for preprocessor-level constants or platform guards.
