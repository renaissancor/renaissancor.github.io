---
title: "C++ Operator Overloading and Custom Memory Management"
---

# C++ Operator Overloading and Custom Memory Management

This post goes deeper on operator overloading beyond the basics covered in cpp-04, adds the mechanics of the `this` pointer, and covers `operator new`/`operator delete` overloading — a foundation for memory pools and leak detectors. Inheritance and polymorphism are covered separately in cpp-05.

---

## The `this` Pointer

Every non-static member function receives an implicit first parameter: a pointer to the object it was called on. That pointer is `this`. The compiler rewrites member-variable accesses through it automatically.

```cpp
class MyClass {
public:
    int m_value;

    void SetValue(int val) {
        this->m_value = val;  // compiler generates this from plain 'm_value = val'
    }
};

int main() {
    MyClass obj;
    obj.SetValue(10);  // &obj is passed as 'this' implicitly
}
```

At the assembly level the object's address is loaded into `ecx` (x86 thiscall) or `rcx` (x64) before the `call` instruction. From the machine's perspective a member function is just a regular function that happens to receive an extra pointer argument.

**`const` member functions and `this`.**  Declaring a function `const` changes the type of `this` from `T*` to `const T*`. The compiler then enforces that no member variable is modified inside the body.

```cpp
class MyClass {
public:
    void func()       { /* this is MyClass*       */ }
    void func() const { /* this is const MyClass*  */ }
};

MyClass       c1;  c1.func();  // non-const version
const MyClass c2;  c2.func();  // const version
```

Both overloads can coexist — the compiler selects based on the constness of the calling object. The pattern is useful for containers that return a mutable reference from a non-`const` object but a read-only reference from a `const` one.

**`this` and `nullptr`.**  Calling a member function through a null pointer is technically undefined behavior, but it reveals how the mechanism works:

```cpp
MyClass* p = nullptr;
p->PrintHelloWorld();  // works if the body never touches 'this->...'
p->PrintValue();       // crashes: dereferences nullptr to read m_value
```

The call itself does not dereference `this`; only an access to a member variable does. This is not a pattern to rely on — it is a diagnostic insight.

**Performance note.**  Every `this->m_member` access is an indirection through a pointer. If a member variable is read many times inside a tight loop, copying it to a local variable at the top of the function eliminates repeated indirections:

```cpp
void MyClass::Process() {
    int a = this->_a;   // one read from memory
    if (a > 10) {
        int b = a * 10; // subsequent reads from register/stack
    }
}
```

A sufficiently optimizing compiler will do this automatically with `-O2`, but being explicit documents intent and avoids surprising behavior at lower optimization levels.

---

## Operator Overloading: Member vs Non-Member

An overloaded operator is a function with the special name `operator@`. It can be defined as a class member or as a free (non-member) function. The choice matters.

**Operators that must be members.**  A small set of operators is required to be non-static member functions because they are inherently tied to the class's own object:

| Operator | Reason |
|----------|--------|
| `=`  | Assignment must control the destination object |
| `[]` | Subscript must return a reference into the object |
| `()` | Function-call operator defines call behavior |
| `->` | Arrow must control pointer-like dereferencing |
| `new` / `delete` | Class-scoped allocation/deallocation |

**Operators that cannot be overloaded at all.**

| Operator | Why |
|----------|-----|
| `.`  `.* ` | Member access — overloading would break the access syntax itself |
| `::`       | Scope resolution — compile-time only |
| `?:`       | Ternary — both branches must be evaluated as a unit |
| `sizeof` / `typeid` | Fixed language semantics |
| `static_cast` / `dynamic_cast` / `reinterpret_cast` / `const_cast` | Casting mechanics |

**Prefer non-member (friend) for symmetric binary operators.**  If the left operand is not always your type — e.g., `std::ostream << MyClass` — define the operator as a `friend` free function:

```cpp
class Vec2 {
public:
    float x, y;
    Vec2(float x, float y) : x(x), y(y) {}

    // Member: left operand is always Vec2
    Vec2 operator+(const Vec2& rhs) const {
        return Vec2(x + rhs.x, y + rhs.y);
    }

    // Non-member friend: left operand is ostream, not Vec2
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v) {
        return os << "(" << v.x << ", " << v.y << ")";
    }
};
```

**Return-by-reference for chaining.**  Operators like `<<`, `=`, and `[]` need to return a reference to the left-hand object so that calls can be chained (`cout << a << b`, `a = b = c`):

```cpp
// operator= canonical form
Vec2& operator=(const Vec2& rhs) {
    if (this != &rhs) { x = rhs.x; y = rhs.y; }
    return *this;   // return reference, not value
}

// operator[] — two overloads for const-correctness
      float& operator[](int i)       { return i == 0 ? x : y; }
const float& operator[](int i) const { return i == 0 ? x : y; }
```

Returning `*this` by reference is what makes `a = b = c` work: `b = c` returns `b` as a reference, then `a = b` assigns from it.

---

## Overloading `operator new` and `operator delete`

### The two-step `new` expression

The expression `T* p = new T(args)` is always two distinct operations:

1. **Memory allocation**: `operator new(sizeof(T))` — equivalent to `malloc(sizeof(T))`, returns a raw `void*`.
2. **Construction**: the constructor is called on the allocated memory.

The compiler desugars `new T(args)` into roughly:

```cpp
void* mem = operator new(sizeof(T));
T* ptr = static_cast<T*>(mem);
ptr->T::T(args);          // constructor call on already-allocated memory
```

These two steps are independently customizable.

### Placement `new`

Placement `new` calls the constructor on memory you already own, without any allocation:

```cpp
Entity* p = static_cast<Entity*>(operator new(sizeof(Entity)));
new(p) Entity();      // construct in-place — no heap allocation
p->~Entity();         // explicit destructor call
new(p) Entity();      // reuse the same memory
p->~Entity();
operator delete(p);   // free exactly once
```

The allocate-once / construct-many pattern is the basis of memory pools. In game server code, an actor pool follows this pattern directly:

```cpp
struct ActorPool {
    Actor*  pool;
    size_t  capacity;

    explicit ActorPool(size_t n) : capacity(n) {
        pool = static_cast<Actor*>(operator new(sizeof(Actor) * capacity));
    }
    ~ActorPool() {
        for (Actor* a : activeActors)
            a->~Actor();        // destroy without freeing
        operator delete(pool);  // free once
    }
};
```

### Overloading `operator new` globally or per-class

`operator new(size_t)` is a regular function — you can replace it. **Always use `malloc` internally**, never `new`, to avoid infinite recursion:

```cpp
void* operator new(size_t size) {
    return malloc(size);  // must NOT call new here
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}
```

For class-scoped control, declare them as static members (they are implicitly static):

```cpp
class MyClass {
public:
    static void* operator new(size_t size);
    static void  operator delete(void* ptr) noexcept;
};
```

The class-scoped versions take precedence over the global ones when allocating/freeing `MyClass` objects.

### Memory leak detection via `operator new` overloading

Because `operator new` is a replaceable function, it is the natural hook for tracking allocations. The standard technique uses a macro to inject source location:

```cpp
// In a debug header
#define new new(__FILE__, __LINE__)

// Matching overload
void* operator new(size_t size, const char* file, int line) {
    void* ptr = malloc(size);
    recordAllocation(ptr, size, file, line);  // store in a map
    return ptr;
}

void operator delete(void* ptr) noexcept {
    recordDeallocation(ptr);  // remove from map
    free(ptr);
}
```

At program exit, anything still in the map is a leak — you have the exact file and line of the allocation. Guard the entire mechanism with `#ifndef NDEBUG` / `#ifdef _DEBUG` so it compiles away in release builds and adds zero overhead in production.

The tracking strategy scales naturally:

1. **Coarse**: watch total memory usage grow over time.
2. **Medium**: count `new`/`delete` calls per type (`Player`, `Monster`) — a mismatch reveals which type leaks.
3. **Fine**: enable the file+line macro to pinpoint the exact allocation site.

---

## `new` vs `malloc` — Key Differences

| | `new` / `delete` | `malloc` / `free` |
|---|---|---|
| Type safety | Returns typed pointer | Returns `void*`, requires cast |
| Constructor/destructor | Called automatically | Not called |
| Overloadable | Yes — `operator new` | No |
| Allocation failure | Throws `std::bad_alloc` | Returns `nullptr` |
| Language | C++ | C |

The practical rule: use `new`/`delete` (or smart pointers) for objects that have constructors and destructors. Use `malloc`/`free` only when you are managing raw memory manually — inside a custom allocator or placement-new pool — and you need the constructor/destructor separation that placement `new` gives you.
