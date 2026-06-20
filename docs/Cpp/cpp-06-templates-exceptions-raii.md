---
title: "C++ Templates, Exceptions, and RAII"
---

# C++ Templates, Exceptions, and RAII

## Inline Functions

The `inline` keyword is a **hint** to the compiler to replace a function call with the function's body directly at the call site. The goal is to eliminate function-call overhead for small, frequently-called functions.

Two things matter in practice:

- `inline` is not a command. The compiler may ignore it entirely and make its own inlining decisions based on optimization settings. At `-O0` / `/Od` even standard-library functions will not be inlined; at `-O2` / `/O2` the compiler inlines aggressively whether or not you wrote the keyword.
- `inline` has a **language-level effect** beyond the hint: it relaxes the One Definition Rule (ODR). Normally a function may be defined in only one translation unit; `inline` allows the same definition to appear in multiple translation units (as long as every definition is identical). This is exactly why small utility functions defined in a header work without triggering a linker error — the `inline` keyword is what permits multiple `.cpp` files to each include and compile that definition.

```cpp
// header.h
inline int Square(int x) { return x * x; }
// Safe to include from any number of .cpp files because of inline's ODR relaxation.
```

STL containers use inline heavily for tiny accessors (`vector::size()`, `vector::begin()`). Without optimization, those calls are not inlined and performance drops noticeably — a reminder that the keyword alone does nothing at `-O0`.

## Templates

Templates let you write **type-generic code** once. The compiler generates a concrete version for each type it sees at the call site — this process is called **instantiation**.

### Function Templates

```cpp
template<typename T>
T Add(T a, T b) {
    return a + b;
}

Add(1, 2);        // compiler generates Add<int>
Add(1.5, 2.5);    // compiler generates Add<double>
```

The compiler deduces `T` from the arguments. You can also specify it explicitly: `Add<int>(1, 2)`. If deduction would be ambiguous — e.g., passing an `int` and a `double` — specify the type explicitly or cast one argument.

### Class Templates

```cpp
template<typename T, int Len>
class SimpleArray {
private:
    T m_arr[Len];
public:
    T& operator[](int idx) { return m_arr[idx]; }
};

SimpleArray<int, 5>  a;  // one class
SimpleArray<int, 7>  b;  // a completely different class
```

`SimpleArray<int, 5>` and `SimpleArray<int, 7>` are treated as **distinct types** — they share no code and no static members. Non-type template parameters like `int Len` must be compile-time constants. Note: each instantiation gets its own independent copy of any static data member.

### Template Specialization

You can override the generic template for a specific type:

```cpp
// General case
template<typename T>
T Max(T a, T b) { return a > b ? a : b; }

// Specialization for const char* — compare by length, not address
template<>
const char* Max(const char* a, const char* b) {
    return strlen(a) > strlen(b) ? a : b;
}
```

The `template<>` prefix with an empty parameter list marks a **full specialization**. The compiler selects the most specific match; calls with `const char*` arguments get the specialized version, everything else gets the generic one.

### Why Templates Must Live in Headers

Templates are instantiated at **compile time**, which means the compiler must see the full template definition — not just a declaration — in every translation unit that uses it. If you split the definition into a `.cpp` file and only put a declaration in the header, the compiler can compile the template file itself but has no definition available when compiling any other file that calls it, causing linker errors.

The standard practice: put the full definition in the `.h` (or `.hpp`) file.

## Exceptions

C++ exception handling uses three keywords: `try`, `throw`, and `catch`.

```cpp
void OpenFile(const std::string& path) {
    if (path.empty())
        throw std::invalid_argument("path cannot be empty");
    // ...
}

try {
    OpenFile("");
} catch (const std::invalid_argument& e) {
    std::cerr << "Error: " << e.what() << '\n';
} catch (...) {
    // catch-all for any other exception type
}
```

`throw` transfers control immediately out of the current function. The runtime searches up the **call stack** for the nearest enclosing `try` block with a matching `catch`. If none is found, `std::terminate()` is called and the program ends.

### Stack Unwinding

When `throw` fires, the runtime **unwinds** every stack frame between the throw site and the matching `catch`. For each frame exited, the destructors of all local objects in that frame are called in reverse construction order. This is the mechanism that makes RAII (discussed below) exception-safe: resources held by local objects are released automatically, even when exceptions bypass your normal cleanup code.

```cpp
void Risky() {
    std::lock_guard<std::mutex> lk(mtx);  // mutex locked here
    MightThrow();                          // throws
    // lk's destructor runs during unwinding — mutex is unlocked
}                                          // even though we never reached here normally
```

### `noexcept`

Marking a function `noexcept` is a promise to the compiler that it will not throw:

```cpp
int SafeAdd(int a, int b) noexcept { return a + b; }
```

If an exception is thrown from a `noexcept` function, the runtime calls `std::terminate()` immediately — no unwinding, no catch. The compiler uses this promise to generate leaner code (it can skip the exception-handling tables for that function).

**Destructors should always be `noexcept`.** An exception escaping a destructor during stack unwinding causes `std::terminate()` because C++ cannot handle two simultaneous exceptions. Modern compilers implicitly mark destructors `noexcept` unless you opt out; mark them explicitly to communicate intent and catch accidents.

### When to Use Exceptions

Exceptions are best reserved for **unrecoverable or truly exceptional conditions** — situations where continued execution is impossible or meaningless: memory allocation failure (`std::bad_alloc`), file not found when the file is required, protocol violations on a network stream.

For routine error paths — a function that may not find a value, malformed user input — prefer return values (`std::optional`, error codes). Overusing exceptions forces every caller into `try/catch`, obscures control flow, and makes it harder to trace error origins.

## RAII

**RAII — Resource Acquisition Is Initialization** — is the single most important C++ idiom for resource safety. The rule is simple:

- Acquire a resource in a constructor.
- Release it in the destructor.
- Let scope and object lifetime do the cleanup automatically.

Because destructors run deterministically when an object goes out of scope — including during exception unwinding — any resource tied to a local object is always released, even when exceptions or early returns bypass your intended cleanup path.

### Why RAII Beats Manual Cleanup

Manual resource management looks like this:

```cpp
void Process() {
    FILE* f = fopen("data.bin", "rb");
    // ... lots of code, maybe an early return, maybe an exception ...
    fclose(f);  // easy to miss
}
```

Every early `return`, every `throw`, every code path that doesn't reach `fclose` is a resource leak. With RAII you move the release into a destructor and the problem disappears:

```cpp
struct FileGuard {
    FILE* m_file;

    explicit FileGuard(const char* path, const char* mode)
        : m_file(fopen(path, mode))
    {
        if (!m_file)
            throw std::runtime_error("failed to open file");
    }

    ~FileGuard() {
        if (m_file) fclose(m_file);
    }

    // Prevent copying — ownership is unique
    FileGuard(const FileGuard&)            = delete;
    FileGuard& operator=(const FileGuard&) = delete;
};

void Process() {
    FileGuard f("data.bin", "rb");
    // f's destructor closes the file on every exit path
}
```

Whether `Process` returns normally, returns early, or throws, `~FileGuard` runs and the file is closed.

### Standard RAII Wrappers

The standard library provides RAII wrappers for the most common resources:

| Resource | RAII wrapper |
|---|---|
| Heap memory | `std::unique_ptr`, `std::shared_ptr` |
| Mutex lock | `std::lock_guard`, `std::unique_lock` |
| File | `std::fstream` (RAII by design) |

`std::lock_guard` is the canonical example from the mutex domain: its constructor calls `mutex.lock()`, its destructor calls `mutex.unlock()`. The lock is released when the guard goes out of scope, even if an exception fires inside the critical section (as shown in the stack unwinding example above).

`std::unique_ptr` replaces raw `new`/`delete` (ownership semantics covered in the pointers post). The RAII angle: heap allocation is the resource, the constructor acquires it with `new`, the destructor releases it with `delete`. You never write `delete` directly.

### RAII and Exception Safety

Stack unwinding calls destructors; RAII destructors release resources. Together they give **strong exception safety** almost for free: if every resource is managed by a local RAII object, no leak is possible regardless of where an exception fires. That is the deeper reason C++ discourages raw `new`/`delete` and manual lock/unlock — not style, but the guarantee exception safety requires.

