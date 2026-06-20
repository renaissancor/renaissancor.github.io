---
title: "C++ Classes, Overloading, and Object Lifetime"
---

# C++ Classes, Overloading, and Object Lifetime

## Class vs Struct & Encapsulation

Both `class` and `struct` define user-defined types with full OOP support — methods, constructors, destructors, and inheritance. The only intrinsic difference is the default access specifier: `struct` members are `public` by default; `class` members are `private`.

```cpp
struct MyData  { int value; };   // value is public
class  CMyData { int value; };   // value is private
```

Encapsulation means exposing a minimal public interface and hiding implementation details behind `private`. This lets you change internal representation without breaking callers, and prevents accidental mutation of internal state.

---

## Const-Correctness

Mark any member function that does not modify the object as `const`. This makes the function callable on `const` objects and `const`-qualified pointers, and it documents intent.

```cpp
class CNewClass {
    int       m_i;
    const int m_i2;
public:
    int GetInt()  const { return m_i; }
    int GetInt2() const { return m_i2; }
    void SetData(int a) { m_i = a; }         // non-const: modifies state

    CNewClass() : m_i(200), m_i2(400) {}     // initializer list required for const member
};
```

`const` members (`m_i2`) cannot be assigned in the constructor body — they must be set in the **member initializer list**. The same rule applies to reference members.

Omitting `const` on a getter means it cannot be called through a `const CNewClass*`, which silently breaks a lot of code at usage sites.

---

## Constructors & Object Lifetime

A constructor runs when an object comes into existence; the destructor runs when it leaves scope (stack) or is explicitly deleted (heap). If you define *any* constructor, the compiler no longer synthesizes a default constructor — you must provide it explicitly if needed.

```cpp
class MyClass {
    float m_Data;
public:
    MyClass()               : m_Data(0.f) {}
    MyClass(float d)        : m_Data(d)   {}
    MyClass(const MyClass& other) : m_Data(other.m_Data) {}
    ~MyClass() {}
};

MyClass a;           // default constructor
MyClass b(3.14f);    // converting constructor
MyClass c(b);        // copy constructor
// MyClass d();      // WRONG — parsed as a function declaration
```

Prefer the initializer list over assignment in the constructor body: it runs before the body executes, is required for `const`/reference members, and is often more efficient.

---

## Copy Mechanics: Copy Constructor & Copy Assignment

The compiler generates shallow-copy versions of both by default. When your class owns a resource (heap memory, file handle), shallow copy causes double-free or use-after-free — define them explicitly (**Rule of Three**, extended to **Rule of Five** in modern C++ to include move operations).

```cpp
class CNew {
    int   m_i;
    short m_s;
public:
    CNew() : m_i(0), m_s(0) {}

    // Copy constructor
    CNew(const CNew& other) : m_i(other.m_i), m_s(other.m_s) {}

    // Copy assignment — must return reference to enable chaining
    CNew& operator=(const CNew& other) {
        m_i = other.m_i;
        m_s = other.m_s;
        return *this;
    }
};

CNew a;
CNew b(a);    // copy constructor
CNew c;
c = a;        // copy assignment
CNew d = c;   // also copy constructor (direct-initialization, not assignment)
```

If you implement a copy constructor but forget the assignment operator (or vice versa), users will hit silent shallow copies through the auto-generated version.

---

## Function & Operator Overloading

C++ resolves overloaded calls at compile time based on argument types. The same name can map to completely different implementations:

```cpp
namespace MY_SPACE {
    void IntChange(int*  p) { *p = 200; }   // pointer version
    void IntChange(int&  r) {  r = 400; }   // reference version
}

int a = 10;
MY_SPACE::IntChange(&a);  // calls pointer overload
MY_SPACE::IntChange(a);   // calls reference overload
```

Operators are overloaded as member functions (or free functions for symmetric operators like `+`). Return `*this` by reference from operators like `<<` and `=` to enable chaining:

```cpp
class CMyDataType {
    int m_Data;
public:
    CMyDataType operator+(CMyDataType other) const {
        CMyDataType result;
        result.m_Data = m_Data + other.m_Data;
        return result;
    }

    CMyDataType& operator<<(int a)           { printf("%d",  a); return *this; }
    CMyDataType& operator<<(float a)         { printf("%f",  a); return *this; }
    CMyDataType& operator<<(void(*fn)(void)) { fn();             return *this; }

    CMyDataType() : m_Data(0) {}
};

void EndL() { printf("\n"); }

// Usage
CMyDataType d;
d << 10 << 3.14f << EndL << 42;   // chains because each << returns *this
```

Returning by value instead of by reference breaks chaining and silently copies on every call — a common oversight.

---

## Namespaces

Namespaces scope names to avoid collisions across translation units and libraries. Access with `::`, or selectively import with `using`:

```cpp
namespace MY_SPACE {
    int g_Int = 0;

    namespace SUB_SPACE {
        void IntPtrChange(int*& p) { p = reinterpret_cast<int*>(10); }
        void IntPtrChange(int** p) { *p = reinterpret_cast<int*>(4);  }
    }
}

using MY_SPACE::SUB_SPACE::IntPtrChange;

int* p = nullptr;
IntPtrChange(p);    // reference-to-pointer overload
IntPtrChange(&p);   // pointer-to-pointer overload
MY_SPACE::g_Int = 100;
```

Avoid `using namespace` at file scope in headers — it pollutes every translation unit that includes the header.
