---
title: C++ Pointers and Memory Management
---

# C++ Pointers and Memory Management

## Pointer Basics

A pointer stores the memory address of another variable. The type of the pointer determines how arithmetic and dereferencing are interpreted.

```cpp
int i = 10;
int* pI = &i;   // & takes the address
*pI = 20;       // * dereferences — writes through the pointer

// Pointer arithmetic: steps by sizeof(T)
int arr[5] = {};
int* p = arr;
p[1] = 99;      // identical to *(p + 1) = 99
```

`void*` accepts any address but cannot be dereferenced or used in arithmetic without a cast first. On 64-bit systems all pointer types are 8 bytes regardless of the pointed-to type.

Double pointers (`T**`) are needed when a function must reassign the caller's pointer, not just the data it points to:

```cpp
void Nullify(int** _pp) { *_pp = nullptr; }

int* p = reinterpret_cast<int*>(0x1);
Nullify(&p);  // p is now nullptr
```

---

## const / constexpr / volatile

`const` prevents reassignment after initialisation. `constexpr` additionally requires the value to be evaluable at compile time. `volatile` suppresses compiler caching, useful for hardware registers or memory that can change outside normal program flow.

```cpp
const int kMax = 100;       // runtime const
constexpr int kStep = 4;    // compile-time const — value inlined by compiler

volatile const int kReg = 0xFF; // re-read from memory on every access
```

Casting away `const` to write through a pointer is **undefined behaviour** — the compiler is permitted to assume the original value never changes and may use a cached register copy even after the write.

---

## Const Pointers and Type-Punning

The position of `const` relative to `*` determines what is read-only:

| Declaration              | Pointer reassignable | Pointed-to value writable |
|--------------------------|:--------------------:|:-------------------------:|
| `const int* p`           | yes                  | no                        |
| `int* const p`           | no                   | yes                       |
| `const int* const p`     | no                   | no                        |

Use `const int*` in function parameters to document read-only intent and allow the compiler to catch accidental writes:

```cpp
void Print(const int* data) {
    printf("%d\n", *data);
    // *data = 0;  // compile error
}
```

**Type-punning** — interpreting the same bytes as a different type — is legal via `unsigned char*` (the only aliasing exemption in the standard) but undefined through arbitrary casts:

```cpp
char raw = -1;
unsigned char* u = reinterpret_cast<unsigned char*>(&raw);
// *u == 255 (unsigned view); raw == -1 (signed view) — same bytes, different interpretation
```

Casting a `char[]` to `int*` and then advancing the pointer mixes element sizes and easily produces misaligned access; avoid it in portable code.

---

## C Memory Functions: memset / memcpy / memmove

All three operate byte-by-byte through a `void*` interface. Under the hood they cast to `unsigned char*` (aliasing-safe) and iterate:

```cpp
// Zero-fill: sets every byte to 0
memset(arr, 0, sizeof(arr));

// Fast copy — regions must NOT overlap
memcpy(dest, src, sizeof(src));

// Safe copy — handles overlapping regions by choosing copy direction
memmove(&data[2], &data[0], 5);
```

`memmove` checks whether destination falls inside the source range and copies backward when it does, preventing self-clobber. The hand-rolled version shows the logic clearly:

```cpp
void MemMove(void* _Dest, int _Size, void* _Src) {
    unsigned char* d = (unsigned char*)_Dest;
    unsigned char* s = (unsigned char*)_Src;
    if (d < s || d >= s + _Size)
        for (int i = 0;         i < _Size; ++i) d[i] = s[i];
    else
        for (int i = _Size - 1; i >= 0;    --i) d[i] = s[i];
}
```

String literals (`const char* p = "hello"`) live in read-only memory. Writing to them is undefined; copy to a `char[]` first if mutation is needed.

---

## Dynamic Allocation: new / delete and the Heap

Stack allocation is automatic but size-limited and scoped to the enclosing block. Heap allocation persists until explicitly freed.

```cpp
// C style
int* p = (int*)malloc(sizeof(int) * n);
free(p);

// C++ style — preferred; invokes constructors/destructors
int* p = new int[n];
delete[] p;
```

Rules:
- Every `new` needs a matching `delete`; every `new[]` needs `delete[]`.
- Accessing memory outside the allocated range is heap corruption — it may not crash immediately, making it difficult to diagnose.
- After `free`/`delete`, set the pointer to `nullptr` to prevent use-after-free.

---

## Applied: Dynamic Array and Linked List

These two structures illustrate the trade-offs of heap allocation in practice.

**Dynamic array** — doubles capacity on overflow, O(1) amortised push, O(1) index:

```cpp
void PushBack(tArray* arr, int val) {
    if (arr->Size == arr->Capacity) {
        int* pNew = (int*)malloc(sizeof(int) * arr->Capacity * 2);
        memcpy(pNew, arr->pArr, sizeof(int) * arr->Size);
        free(arr->pArr);
        arr->pArr = pNew;
        arr->Capacity *= 2;
    }
    arr->pArr[arr->Size++] = val;
}
```

**Singly linked list** — O(1) front insertion, O(N) traversal; each node is individually heap-allocated:

```cpp
void PushFront(tList* list, int val) {
    tNode* node = (tNode*)malloc(sizeof(tNode));
    node->Data  = val;
    node->pNext = list->pHead;
    list->pHead = node;
    ++list->Size;
}
```

Cleanup must walk every node — `free(list->pHead)` alone leaks every subsequent node:

```cpp
void Release(tList* list) {
    tNode* cur = list->pHead;
    while (cur) {
        tNode* next = cur->pNext;
        free(cur);
        cur = next;
    }
    list->pHead = nullptr;
    list->Size  = 0;
}
```

| Structure     | Index | Push front | Push back (amortised) | Memory overhead |
|---------------|:-----:|:----------:|:---------------------:|:---------------:|
| Dynamic array | O(1)  | O(N)       | O(1)                  | low (contiguous)|
| Linked list   | O(N)  | O(1)       | O(N) without tail ptr | per-node alloc  |
