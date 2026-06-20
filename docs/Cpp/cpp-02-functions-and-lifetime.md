---
title: "C++ Functions, Recursion, and Variable Lifetime"
---

# C++ Functions, Recursion, and Variable Lifetime

## Structs and User-Defined Types

C++ structs group related data under a single type. In C++ (unlike C), structs may also carry member functions and default to `public` access.

```cpp
struct Player {
    int MaxHP, CurHP;
    int Att, Def, Dex, Agi, Int;
};
```

Nested structs compose types naturally:

```cpp
struct NewType {
    MyType type;
    int Age;
    int Height;
};
```

`typedef` creates aliases and predates `using`. In modern C++, prefer `using`:

```cpp
// C-style
typedef struct Monster { int i; char c; short s; } MONSTER;

// Modern C++
using MONSTER = Monster;
```

Default arguments reduce call-site noise. They must be declared right-to-left in the parameter list:

```cpp
int Add(int a, int b, int c = 0) { return a + b + c; }

Add(10, 20);     // c defaults to 0
Add(10, 20, 5);  // c = 5
```

---

## Recursion

A recursive function calls itself toward a base case. Every recursive call pushes a new stack frame; without a base case the stack exhausts and crashes.

```cpp
// Termination condition is mandatory
void CountDown(int n) {
    if (n == 0) return;
    CountDown(n - 1);
}
```

### Factorial: recursive vs iterative

```cpp
// Recursive — readable, O(n) stack depth
int Factorial_R(int n) {
    if (n == 1) return 1;
    return Factorial_R(n - 1) * n;
}

// Iterative — same result, O(1) stack
int Factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}
```

### Fibonacci: why the recursive form is slow

```cpp
// O(2^n) — recalculates subproblems repeatedly
int Fib_R(int n) {
    if (n <= 2) return 1;
    return Fib_R(n - 1) + Fib_R(n - 2);
}

// O(n) — iterative, no repeated work
int Fib(int n) {
    if (n <= 2) return 1;
    int a = 1, b = 1, val = 0;
    for (int i = 0; i < n - 2; ++i) { val = a + b; a = b; b = val; }
    return val;
}
```

Use recursion when it matches the problem's natural structure (trees, graphs) and stack depth is bounded. Prefer iteration otherwise.

---

## Storage Duration and Static Variables

Every variable has a *storage duration* that determines when its memory is allocated and freed.

| Storage duration | Keyword / location | Memory region | Lifetime |
|---|---|---|---|
| Automatic | local variable | Stack | Until enclosing scope exits |
| Static (local) | `static` inside a function | Data segment | Entire program |
| Static (file) | `static` at file scope | Data segment | Entire program |
| Dynamic | `malloc` / `new` | Heap | Until `free` / `delete` |

### Function-local static

Initialized exactly once on first call; retains its value across all subsequent calls:

```cpp
int Counter() {
    static int count = 0;   // initialized once
    return ++count;
}

Counter(); // 1
Counter(); // 2
Counter(); // 3
```

Because the variable accumulates state, functions using it are not pure and not thread-safe by default.

You can return a pointer to a local static — the address remains valid for the program's lifetime:

```cpp
int* GetCounter() {
    static int n = 0;
    ++n;
    return &n;   // valid: static outlives the function
}
```

Returning a pointer to a plain local variable is undefined behavior — the variable is destroyed when the function returns.

### Global vs. file-static globals

A plain global is visible across all translation units. A `static` global is confined to the file that declares it:

```cpp
int    g_Shared = 0;   // visible everywhere
static int g_Local = 0; // this file only
```

Declaring a non-static global in a header and including it in multiple `.cpp` files produces a **multiple-definition linker error**. The pattern to share a global is:

```cpp
// header (declaration only)
extern int g_Shared;

// exactly one .cpp (definition)
int g_Shared = 0;
```

### Scope shadowing with `::`

A local variable with the same name as a global shadows it. The scope-resolution operator `::` bypasses the shadow:

```cpp
int g_A = 0;

void Foo() {
    int g_A = 100;   // shadows global
    ++::g_A;         // increments the global
}
```

---

## Memory Layout: Stack, Heap, Data, and Code

A C/C++ process divides memory into four regions:

```
Region        Contents                         Example
──────────    ─────────────────────────────    ─────────────────
Stack         Local variables, return addrs    int x;
Heap          Runtime-allocated blocks         malloc() / new
Data segment  Global and static variables      static int n;
Code (ROM)    Executable instructions,         function bodies,
              string literals, constants       "hello"
```

Key rules that follow directly from this layout:

- **Stack variables** vanish when their function returns. Never return their address.
- **Data-segment variables** (global, static) live for the full program run. Addresses are always safe to hold.
- **Heap blocks** live until explicitly freed. Forgetting `free`/`delete` leaks memory; accessing after free is undefined behavior.
- String literals (`"hello"`) live in the code/ROM region and must not be written to.
