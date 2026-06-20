# Linked Lists and Iterators

A focused reference on doubly linked list implementation and custom iterator design in C++, covering the concrete mechanics that trip people up: pointer rewiring during erase, iterator state encoding, the reverse function, and the invalidation rules that differ between `vector` and `list`.

---

## Vector vs. List: Trade-offs That Matter

Both `std::vector` and `std::list` are sequences, but their underlying structures make them good at opposite things.

| Operation | `vector` | `list` |
|---|---|---|
| Random access `[i]` | O(1) | Not supported |
| `push_back` | Amortized O(1) | O(1) |
| `push_front` / `pop_front` | O(n) — not provided | O(1) |
| Insert / erase at middle | O(n) shift | O(1) pointer rewire |
| Cache locality | Contiguous — excellent | Pointer-chased — poor |

The cache-locality gap is real: iterating a `vector<int>` of 10 million elements is dramatically faster than a `list<int>` of the same size because vector elements live in consecutive memory and the CPU prefetcher works. A linked list earns its keep when you need O(1) splice or frequent interior insertions without invalidating other iterators.

`vector` deliberately omits `pop_front()`. Removing the first element requires shifting every subsequent element one position to the left — O(n) — which would mislead callers who expect front operations to be cheap.

---

## Iterator Invalidation: The Critical Difference

This is where bugs live.

**Vector erase** invalidates the erased iterator *and every iterator after it*, because the elements following the gap are shifted left in memory. Anything pointing past the erasure point is now off by one.

**List erase** invalidates *only the erased iterator*. The node is unlinked and freed; every other node stays where it is. Iterators to surviving nodes remain valid across insertions and erasures anywhere in the list.

### Safe erase-in-loop pattern

Both containers follow the same idiom: `erase` returns the next valid iterator, so assign it back rather than incrementing the now-invalid one.

```cpp
// Works for both std::vector and std::list
for (auto it = container.begin(); it != container.end(); ) {
    if (*it < 60)
        it = container.erase(it);  // erase returns next; do NOT ++it here
    else
        ++it;
}
```

The bad pattern — incrementing after erasing — is undefined behavior on a vector and a null-dereference on a list:

```cpp
// WRONG
for (auto it = vec.begin(); it != vec.end(); ++it) {
    if (*it % 2 == 0)
        vec.erase(it);  // it is now invalid; ++it on next iteration is UB
}
```

---

## Doubly Linked List: Node and Container Structure

```cpp
template<typename T>
struct tNode {
    T       m_Data;
    tNode*  m_Next;
    tNode*  m_Prev;

    tNode() : m_Data(), m_Next(nullptr), m_Prev(nullptr) {}
    tNode(const T& _Data, tNode* _Next = nullptr, tNode* _Prev = nullptr)
        : m_Data(_Data), m_Next(_Next), m_Prev(_Prev) {}
};
```

The container keeps `m_Head`, `m_Tail`, and `m_Size`. Storing `m_Tail` is what makes `push_back` O(1) — without it you would traverse to the last node on every insertion.

```cpp
template<typename T>
class CList {
private:
    tNode<T>* m_Head;
    tNode<T>* m_Tail;
    int       m_Size;
public:
    void push_back(const T& _Data);
    void push_front(const T& _Data);
    void pop_front();
    void pop_back();
    // ...
    class iterator;
    iterator begin();
    iterator end();
    iterator erase(iterator& _iter);
};
```

### push / pop mechanics

`push_back` when empty sets both `m_Head` and `m_Tail` to the new node. When non-empty: link current tail's `m_Next` to the new node, set new node's `m_Prev` to current tail, update `m_Tail`.

`pop_front` when the list has exactly one element must reset *both* `m_Head` and `m_Tail` to `nullptr`. Forgetting the tail reset leaves a dangling pointer that corrupts the next `push_back`.

`pop_back` on a one-element list can just delegate to `pop_front` to avoid duplicating the single-node edge case.

---

## Erase: Pointer Rewiring

`erase` on a list is three cases:

1. **Head node** — delegate to `pop_front()`.
2. **Tail node** — delegate to `pop_back()`.
3. **Interior node** — rewire the surrounding nodes:

```cpp
// middle node case (simplified)
tNode<T>* prev = _iter.m_TargetNode->m_Prev;
tNode<T>* next = _iter.m_TargetNode->m_Next;
prev->m_Next = next;
next->m_Prev = prev;
delete _iter.m_TargetNode;
--m_Size;
```

The function asserts that the iterator's owner matches `this` before touching anything:

```cpp
assert(_iter.m_Owner == this);
```

Passing an iterator from a different container would silently corrupt foreign memory without this check.

---

## Custom Iterator Design

### State encoding

The list iterator carries two fields: `m_Owner` (pointer to the container) and `m_TargetNode` (pointer to the current node). Three meaningful states:

| `m_Owner` | `m_TargetNode` | Meaning |
|---|---|---|
| `nullptr` | `nullptr` | Invalid / default-constructed |
| non-null | non-null | Points to a valid node |
| non-null | `nullptr` | `end()` — one past the last element |

`end()` is *not* a node — it is an iterator whose `m_TargetNode` is `nullptr` but whose `m_Owner` is set. This matters for `operator--`: decrementing `end()` is legal and must return the tail.

```cpp
class iterator {
    CList<T>* m_Owner;
    tNode<T>* m_TargetNode;
public:
    T& operator*();
    bool operator==(const iterator& o);
    bool operator!=(const iterator& o);
    iterator& operator++();       // prefix
    iterator  operator++(int);    // postfix
    iterator& operator--();       // prefix
    iterator  operator--(int);    // postfix
};
```

### Increment and decrement

`operator++` must assert `m_TargetNode != nullptr` — incrementing `end()` is undefined:

```cpp
iterator& operator++() {
    assert(m_TargetNode != nullptr);  // cannot increment end()
    m_TargetNode = m_TargetNode->m_Next;
    return *this;
}
```

`operator--` handles the `end()` case explicitly by walking back to `m_Tail`:

```cpp
iterator& operator--() {
    assert(m_Owner->m_Head != m_TargetNode);  // cannot decrement begin()
    if (m_TargetNode == nullptr)
        m_TargetNode = m_Owner->m_Tail;       // end() -- => tail
    else
        m_TargetNode = m_TargetNode->m_Prev;
    return *this;
}
```

### Prefix vs. postfix: why it matters

Prefix `++iter` returns `iterator&` — no copy. Postfix `iter++` must return the pre-increment value by copy, then advance the iterator:

```cpp
iterator operator++(int) {          // postfix
    iterator temp = *this;          // copy current state
    ++(*this);                      // advance via prefix
    return temp;                    // return old state
}
```

The postfix signature takes a dummy `int` parameter solely to distinguish the two overloads at the call site. Prefer prefix in loops — postfix pays for a copy you don't use.

A subtle error: `operator++(int)` must return `iterator` by value, not `iterator&`. Returning a reference to `temp` would be a dangling reference to a local variable.

### CArray iterator: index-based variant

For comparison, `CArray<T>` (a dynamic array) uses an index instead of a node pointer:

```cpp
iterator() : m_Owner(nullptr), m_Idx(-1) {}
iterator(CArray<T>* owner, int idx) : m_Owner(owner), m_Idx(idx) {}

iterator begin() {
    return (m_CurSize == 0) ? end() : iterator(this, 0);
}
iterator end() {
    return iterator(this, -1);  // -1 signals end
}
```

The array `erase` shifts elements left and nullifies the passed iterator:

```cpp
iterator CArray<T>::erase(iterator& _TargetIter) {
    assert(this == _TargetIter.m_Owner);
    assert(_TargetIter.m_Idx != -1 && _TargetIter.m_Idx < m_CurSize);

    iterator nextIter(this, _TargetIter.m_Idx);  // position stays, element shifts in

    int moveCount = m_CurSize - (_TargetIter.m_Idx + 1);
    for (int i = 0; i < moveCount; ++i)
        m_Data[_TargetIter.m_Idx + i] = m_Data[_TargetIter.m_Idx + i + 1];

    --m_CurSize;
    _TargetIter.m_Owner = nullptr;
    _TargetIter.m_Idx   = -1;     // poison the caller's iterator
    return nextIter;
}
```

This poisons the original iterator after erasure (sets it to the invalid state) rather than leaving it silently stale.

---

## Reverse: Recursive Pointer Swap

The reverse function walks to the last node recursively, swaps `m_Head`/`m_Tail` at the base case, then unwinds flipping each node's `m_Next` and `m_Prev`:

```cpp
void reverse() {
    reverse(m_Head);
}

void reverse(tNode<T>* _Node) {
    if (_Node->m_Next != nullptr)
        reverse(_Node->m_Next);      // recurse to the end

    if (_Node->m_Next == nullptr) {  // base case: last node
        tNode<T>* pTemp = m_Head;
        m_Head = m_Tail;
        m_Tail = pTemp;
    }

    // swap this node's pointers
    tNode<T>* pTemp  = _Node->m_Next;
    _Node->m_Next    = _Node->m_Prev;
    _Node->m_Prev    = pTemp;
}
```

Walk-through on a 3-node list `[A → B → C]`:

1. Recurse all the way to C.
2. At C: `m_Next == nullptr` → swap head/tail so `m_Head = C`, `m_Tail = A`.
3. Swap C's pointers: `C->m_Next = B`, `C->m_Prev = nullptr`.
4. Unwind to B: swap — `B->m_Next = A`, `B->m_Prev = C`.
5. Unwind to A: swap — `A->m_Next = nullptr`, `A->m_Prev = B`.
6. Result: `[C → B → A]` with correct head and tail.

One off-by-one to watch: the head/tail swap happens *at the last node* (the base case check `m_Next == nullptr`), not after returning from recursion. Placing it after the recursive call but *inside* the nullptr check ensures it runs exactly once.

---

## `friend class` for Iterator Access

The iterator needs to read private fields of its container (`m_Head`, `m_Tail`) for the `operator--` end-case. Declare `friend class` inside the iterator to grant access:

```cpp
class iterator {
    friend class CList<T>;  // CList can access iterator's private members
    friend class DLList<T>; // and vice versa
    // ...
};
```

Without this, `m_Owner->m_Tail` inside `operator--` would be a compile error.
