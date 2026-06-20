---
title: "Dynamic Arrays and STL Vector"
---

# Dynamic Arrays and STL Vector

## C++ Dynamic Memory: `new` / `delete` vs `malloc` / `free`

Raw `malloc` allocates bytes but never calls constructors or destructors — using it with C++ class objects produces uninitialized data and silent resource leaks.

```cpp
// Bad: allocates bytes only, no constructor called
MyClass* p = (MyClass*) malloc(sizeof(MyClass));
free(p); // No destructor called

// Correct: new = malloc + constructor, delete = destructor + free
MyClass* p = new MyClass();
delete p;

// Arrays: always pair new[] with delete[]
MyClass* arr = new MyClass[10];
delete[] arr; // delete (no []) calls destructor only on the first element
```

Stack objects call the constructor at scope entry and the destructor at scope exit automatically — no manual cleanup needed.

---

## Implementing a Dynamic Array: Growth and Reallocation

A dynamic array maintains three values: a heap-allocated buffer, the current element count (`size`), and the allocated slot count (`capacity`). When `size == capacity`, a new buffer must be allocated, all existing elements copied (or moved), and the old buffer freed.

A template-based implementation that mirrors `std::vector`:

```cpp
template <typename T>
class CArray {
    T*     m_data;
    size_t m_Size;
    size_t m_Capacity;

public:
    CArray(size_t capacity = 1)
        : m_data(new T[capacity]), m_Size(0), m_Capacity(capacity) {}

    ~CArray() { delete[] m_data; }

    void push_back(const T& val) {
        if (m_Size == m_Capacity) {
            size_t newCap = m_Capacity * 2;
            T* newData    = new T[newCap];
            for (size_t i = 0; i < m_Size; ++i)
                newData[i] = m_data[i];
            delete[] m_data;
            m_data     = newData;
            m_Capacity = newCap;
        }
        m_data[m_Size++] = val;
    }

    T& operator[](size_t idx) {
        assert(idx < m_Size);
        return m_data[idx];
    }
};
```

Key design constraints:
- The Rule of Three/Five applies: if you define a destructor, you must also define (or delete) the copy constructor and copy-assignment operator — otherwise two `CArray` instances will share the same raw pointer and double-free on destruction.
- `assert(idx < m_Size)` fires during debug builds; in release it compiles away.

---

## `std::vector`: Size, Capacity, and Growth

`std::vector` wraps the same pattern. Its object overhead is exactly **three pointers** (pointer to data, pointer to end-of-size, pointer to end-of-capacity), regardless of element type:

| Platform | `sizeof(vector<int>)` | `sizeof(vector<double>)` |
|---|---|---|
| x86-32 | 12 bytes | 12 bytes |
| x86-64 | 24 bytes | 24 bytes |

The object size is always `3 × sizeof(void*)`. The heap buffer grows separately.

### Growth factor: compiler-dependent

The growth ratio is **not standardized**:

- **MSVC** uses approximately **×1.5**
- **GCC / Clang** use **×2**

Measured on MSVC (VS 2022) pushing 30 elements into an empty `vector<int>`:

```
Size:  1, Capacity:  1
Size:  2, Capacity:  2
Size:  3, Capacity:  3
Size:  4, Capacity:  4
Size:  5, Capacity:  6   ← first ×1.5 growth
Size:  7, Capacity:  9
Size: 10, Capacity: 13
Size: 14, Capacity: 19
Size: 20, Capacity: 28
Size: 29, Capacity: 42
```

Each reallocation copies all existing elements — O(n) work per event. Because the capacity at least doubles geometrically (×1.5 or ×2), each element is copied O(log n) times total, giving **amortized O(1) `push_back`**.

### `reserve()` vs `resize()`

| Operation | What changes | Initializes elements? |
|---|---|---|
| `reserve(n)` | capacity only | No |
| `resize(n)` | size and capacity | Yes — default-constructs new elements |

Use `reserve()` when you know the final count upfront — it eliminates all mid-loop reallocations with zero size change.

```cpp
vector<int> v;
v.reserve(1000); // One allocation; no element added yet
for (int i = 0; i < 1000; ++i)
    v.push_back(i); // Never reallocates
```

### Complexity summary

| Operation | Complexity | Why |
|---|---|---|
| `operator[]` / `at()` | O(1) | Direct pointer offset |
| `push_back` | O(1) amortized | Occasional O(n) realloc amortizes over n pushes |
| Insert / erase at index `i` | O(n − i) | Must shift all trailing elements |
| Insert / erase at front | O(n) | Shifts the entire buffer |

The front-insert cost is why `std::vector` has no `push_front()`. Use `std::deque` or `std::list` if you need frequent front operations.

---

## Iterator Mechanics and Invalidation

### What iterators actually are

Vector iterators are thin wrappers around raw pointers into the heap buffer:

```cpp
printf("vec.data()      : %p\n", (void*)vec.data());
printf("&*vec.begin()   : %p\n", (void*)&*vec.begin());   // same address
printf("&*vec.end()     : %p\n", (void*)&*vec.end());     // data + size
printf("&*vec.rbegin()  : %p\n", (void*)&*vec.rbegin());  // data + size - 1
printf("&*vec.rend()    : %p\n", (void*)&*vec.rend());    // data - 1
```

`begin()` points to `data[0]`. `end()` points one past the last element (`data + size`) — it is a valid address but **must never be dereferenced**. `rbegin()` points to the last element; `rend()` points one before the first.

### Invalidation on reallocation

Any operation that triggers reallocation — `push_back` when `size == capacity`, `insert`, `resize` above capacity — **invalidates all existing iterators, pointers, and references** into the vector. The old buffer is freed; your iterator now points at freed memory.

```cpp
vector<int> v = {1, 2, 3};
auto it = v.begin(); // points into the current buffer

v.push_back(4);      // may reallocate
// *it is now undefined behavior — old buffer may be freed
```

Fix: either re-acquire the iterator after the modifying call, or use `reserve()` beforehand to guarantee no reallocation occurs.

### Safe erase in a loop

`erase()` invalidates the erased element and all iterators after it. The return value is a valid iterator to the element that now occupies the erased position — always use it:

```cpp
for (auto it = v.begin(); it != v.end(); ) {
    if (*it <= 60)
        it = v.erase(it); // erase returns the next valid iterator
    else
        ++it;
}
```

The same pattern applies to `std::list`, where `erase()` also invalidates only the erased node's iterator (not others, unlike vector).

### Empty-container guard

When a container is empty, `begin() == end()`. Dereferencing either is undefined behavior:

```cpp
vector<int> v;
auto it = v.begin();
if (it == v.end()) { /* container is empty — do not dereference */ }
```

---

## Templates for Reusability: Array Passing

Passing an array by reference allows the compiler to deduce its size, but generates a separate function instantiation per size — O(distinct sizes) code copies:

```cpp
template<int N>
void fill(int (&arr)[N]) { /* ... */ } // new instantiation for [10], [20], etc.
```

Prefer pointer + explicit size; one function handles all sizes without code bloat:

```cpp
void fill(int* arr, int n) { for (int i = 0; i < n; ++i) arr[i] = i; }
```

---

## STL Container Object Sizes at a Glance

Selected results from `sizeof()` on x86-64 (GCC/Clang):

| Container | 32-bit | 64-bit |
|---|---|---|
| `vector<int>` | 12 | 24 |
| `list<int>` | 8 | 16 |
| `deque<int>` | 20 | 40 |
| `set<int>` | 8 | 16 |
| `map<int,int>` | 8 | 16 |
| `unordered_set<int>` | 32 | 64 |
| `unordered_map<int,int>` | 32 | 64 |
| `string` | 24 | 32 |
| `unique_ptr<int>` | 4 | 8 |
| `shared_ptr<int>` | 8 | 16 |

Note that `vector<bool>` is a special case — its object size is 16 / 32 bytes (not 12 / 24), because the standard specialization packs bits and needs extra bookkeeping. `array<int, 13>` is always 52 bytes on both platforms because it has no metadata — it is exactly `13 × sizeof(int)`.
