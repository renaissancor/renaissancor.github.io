---
title: "C++ Inheritance, Polymorphism, and the Singleton Pattern"
---

# C++ Inheritance, Polymorphism, and the Singleton Pattern

## Inheritance and Function Overriding

Derived classes inherit members from their base class and can **override** a base class function by redefining it with the same signature. This is distinct from **overloading**, which defines multiple functions with the same name but different parameter lists within the same scope.

```cpp
class Base {
public:
    void Output() { std::cout << "Base\n"; }
};

class Child : public Base {
public:
    void Output() { std::cout << "Child\n"; }  // overrides Base::Output
};

Child c;
c.Output();        // Child
c.Base::Output();  // Base — explicit scope access still works
```

A base class pointer can point to a derived class object, but not the reverse. The derived object contains the full base subobject at its start, so a base pointer safely addresses it. A child pointer to a base object would be unsafe — the extra members the child expects simply are not there.

```cpp
Parent* p = &childObj;  // valid
// Child* c = &parentObj;  // compile error
```

## Virtual Functions and Dynamic Dispatch

Without `virtual`, calling through a base pointer always resolves to the base version at **compile time** (static binding). Adding `virtual` switches to **runtime dispatch**: the correct override is selected based on the actual object type, not the pointer type.

```cpp
class Actor {
public:
    virtual void Tick() { /* base */ }
    virtual ~Actor() {}
};

class Player : public Actor {
public:
    void Tick() override { /* player logic */ }
};

class Monster : public Actor {
public:
    void Tick() override { /* monster logic */ }
};

std::vector<Actor*> actors;  // holds any mix of derived types
for (Actor* a : actors)
    a->Tick();  // calls the right override every time
```

The `override` specifier (C++11) is not required but is strongly recommended: the compiler will error if the function does not actually match a virtual signature in a base class, catching typos and signature mismatches early.

Always declare the base class destructor `virtual` in polymorphic hierarchies. Without it, `delete basePtr` on a derived object calls only the base destructor, leaking whatever the derived destructor would have released.

### Copy Constructors in Derived Classes

The compiler-generated copy constructor for a derived class calls the base **default** constructor, not the base copy constructor — so the base subobject is zero-initialized instead of copied. Fix it explicitly:

```cpp
class Child : public Base {
public:
    long long m_Child;

    Child(const Child& other)
        : Base(other),          // forward to Base copy constructor
          m_Child(other.m_Child)
    {}
};
```

## Polymorphism and vtables

Every class with at least one virtual function gets a **virtual function table (vtable)** — a compiler-generated array of function pointers, one slot per virtual method. Each object of that class carries a hidden `__vfptr` pointing to its class's vtable.

When a virtual call fires through a base pointer:

1. Dereference `__vfptr` to reach the vtable.
2. Index into the table at the slot for that function.
3. Call the pointer stored there — which is the most-derived override.

Consequence: objects of classes with virtual functions are larger than their data members alone because of the `__vfptr`.

```cpp
class NoVirtual { float v; };        // sizeof = 4
class WithVirtual { float v; virtual void f() {} };  // sizeof = 16 (on x64)
```

Derived vtables are copied from the base and then updated: overridden slots point to the derived implementation; non-overridden slots keep the base pointer.

**Object slicing** is the pitfall on the other side: assigning a derived object *by value* to a base object strips the derived part entirely. Polymorphism requires pointers or references — never bare value copies.

## Pure Virtual Functions and Abstract Classes

Appending `= 0` to a virtual declaration makes it **pure virtual**: no body in this class, and every concrete subclass must provide one. A class with any pure virtual function is **abstract** and cannot be instantiated directly.

```cpp
class GameObject {
public:
    virtual void Update(float dt) = 0;
    virtual void Render()        = 0;
    virtual ~GameObject() {}
};

class Player : public GameObject {
public:
    void Update(float dt) override { /* ... */ }
    void Render()         override { /* ... */ }
};
```

If a derived class omits any pure virtual override, it too becomes abstract.

## Static and `const` in Class Context

### `const` member functions

Appending `const` after the parameter list promises the function will not modify the object. Only `const` member functions may be called through a `const` pointer or reference.

```cpp
class Counter {
    int m_count;
public:
    void Increment()     { ++m_count; }
    int  Get()     const { return m_count; }  // read-only guarantee
};

const Counter* p = &c;
p->Get();        // ok
// p->Increment(); // compile error
```

`const` member variables must be initialized in the member-initializer list; they cannot be assigned in the constructor body.

### Static members

A `static` member variable belongs to the class, not to any instance. One copy lives in the data segment, shared across every object.

```cpp
class CTest {
public:
    static int s_count;
private:
    int m_id;
};
int CTest::s_count = 0;  // definition required in exactly one .cpp
```

A `static` member function has no `this` pointer and therefore cannot access non-static members. In return, it can be called without any instance (`CTest::StaticFunc()`). This scoping — class namespace plus access to private statics — is its main advantage over a free function.

## The Singleton Pattern

A Singleton ensures a class has exactly one instance and provides a global access point to it. In game engines it is typically used for manager objects (audio manager, resource manager, engine core).

### Canonical implementation — Meyers Singleton

The cleanest modern C++ form uses a **static local variable** inside the accessor. C++11 guarantees that static local initialization is thread-safe and happens exactly once.

```cpp
class Engine {
public:
    static Engine& Get() {
        static Engine instance;  // initialized once, destroyed at exit
        return instance;
    }

    // delete copy/move so the contract is enforced at compile time
    Engine(const Engine&)            = delete;
    Engine& operator=(const Engine&) = delete;

private:
    Engine() = default;
    ~Engine() = default;
};

// Usage
Engine::Get().SomeMethod();
```

No explicit `Destroy()` call is needed — the static local is destroyed in reverse construction order at program exit, which is usually correct for singletons.

The older alternative — a `static` pointer initialized to `nullptr`, allocated on first call, and freed via an explicit `Destroy()` — is still common in older codebases. Its main advantage is explicit lifetime control; its disadvantages are the manual cleanup requirement and the fact that the naive version is not thread-safe without a mutex or `std::call_once`.

### When (and when not) to use it

**Use** when a single shared resource genuinely must exist for the process lifetime: a rendering device, a log sink, a main game clock.

**Avoid** when:

- The class has significant state that you want to reset or replace during tests — singletons resist test isolation.
- Multiple independent subsystems need the same *type* of resource but different *instances* — dependency injection scales better.
- You are reaching for it out of convenience rather than necessity — it is a global variable with extra steps.

The pattern's real cost is hidden coupling: any code anywhere can call `Engine::Get()`, making dependencies invisible. Keep singleton use narrow and document it.
