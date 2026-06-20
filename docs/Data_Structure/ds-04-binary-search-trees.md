---
title: Binary Search Trees
---

# Binary Search Trees

A Binary Search Tree (BST) is a node-based data structure that enforces a strict ordering invariant: for every node, all keys in the left subtree are smaller, and all keys in the right subtree are larger. This invariant makes search, insert, and erase all O(log n) on a balanced tree — and O(n) on a degenerate one.

---

## Enums for Pointer Indexing

Before the node structure, it is worth pinning down why the implementation uses an enum at all. C++ `enum` defines named integer constants that are type-checked at compile time and visible in debuggers with their symbolic names — unlike `#define` macros, which are erased by the preprocessor before any type information exists.

```cpp
// Macro approach — no type safety, invisible in debugger
#define PARENT 0
#define LCHILD 1
#define RCHILD 2

// Enum approach — type-checked, debugger-friendly
enum NODE_TYPE { PARENT, LCHILD, RCHILD };
```

C++11 `enum class` goes further by scoping values so that two enums can share names without collision:

```cpp
enum class Direction { LEFT, RIGHT };   // Direction::LEFT
enum class ChildSide { LEFT, RIGHT };   // ChildSide::LEFT — no conflict
```

For the BST implementation, a plain `enum` is sufficient because `NODE_TYPE` values are used directly as array indices (`arrPtr[LCHILD]`). An `enum class` would require an explicit cast to `int` at every access site, adding noise without benefit here.

The payoff: traversal code that needs to "go the other direction" can store a `NODE_TYPE` variable and flip it between `LCHILD` and `RCHILD` rather than duplicating entire branches of logic.

---

## Node Structure and Class Layout

Each node stores a key-value pair and three pointers indexed by `NODE_TYPE`: `PARENT` (0), `LCHILD` (1), `RCHILD` (2). Using an enum-indexed array instead of named fields keeps traversal code uniform — direction becomes a runtime variable rather than a hard-coded branch.

```cpp
enum NODE_TYPE { PARENT, LCHILD, RCHILD };

template<typename T1, typename T2>
struct Node {
    Pair<T1, T2> pair;
    Node* arrPtr[3];

    Node() : pair(), arrPtr{nullptr, nullptr, nullptr} {}
    Node(const Pair<T1, T2>& _pair, Node* _parent, Node* _lchild, Node* _rchild)
        : pair(_pair), arrPtr{_parent, _lchild, _rchild} {}
};

template<typename T1, typename T2>
class BSTree {
private:
    Node<T1, T2>* m_Root;
    int            m_Size;
public:
    BSTree() : m_Root(nullptr), m_Size(0) {}
};
```

---

## Insert

The first node inserted becomes the root. Every subsequent node walks down from the root, branching left if its key is smaller and right if larger. Duplicate keys trigger `assert(false)` — the same contract `std::map` imposes (silently ignoring a duplicate would leak the newly allocated node).

```cpp
void BSTree<T1, T2>::insert(const Pair<T1, T2>& _pair) {
    Node<T1, T2>* newNode = new Node<T1, T2>(_pair);
    if (!m_Root) {
        m_Root = newNode;
        ++m_Size;
        return;
    }

    Node<T1, T2>* cur = m_Root;
    while (true) {
        if (newNode->pair.key < cur->pair.key) {
            if (!cur->arrPtr[LCHILD]) {
                cur->arrPtr[LCHILD]    = newNode;
                newNode->arrPtr[PARENT] = cur;
                ++m_Size;
                break;
            }
            cur = cur->arrPtr[LCHILD];
        } else if (newNode->pair.key > cur->pair.key) {
            if (!cur->arrPtr[RCHILD]) {
                cur->arrPtr[RCHILD]    = newNode;
                newNode->arrPtr[PARENT] = cur;
                ++m_Size;
                break;
            }
            cur = cur->arrPtr[RCHILD];
        } else {
            assert(false); // duplicate key
        }
    }
}
```

---

## In-Order Successor and Predecessor

These helpers underpin both the iterator's `++`/`--` operators and the two-children erase case.

**Successor** (next larger key):

- If the node has a right child: descend to the **leftmost** node of the right subtree.
- Otherwise: climb until you arrive at the tree via a left-child link; that parent is the successor.

```cpp
Node<T1, T2>* BSTree<T1, T2>::GetInorderSuccessor(Node<T1, T2>* node) {
    if (node->arrPtr[RCHILD]) {
        Node<T1, T2>* s = node->arrPtr[RCHILD];
        while (s->arrPtr[LCHILD]) s = s->arrPtr[LCHILD];
        return s;
    }
    while (node->arrPtr[PARENT] && node == node->arrPtr[PARENT]->arrPtr[RCHILD])
        node = node->arrPtr[PARENT];
    return node->arrPtr[PARENT]; // nullptr if node was maximum
}
```

**Predecessor** is the mirror: rightmost of the left subtree, or climb until arriving via a right-child link.

---

## Iterator: In-Order Traversal Made Incremental

A range-for loop over a BST must visit nodes in sorted order without building a separate list. The iterator stores a pointer to the current node and a pointer back to the owning tree.

**`begin()`** — find the leftmost (minimum) node:

```cpp
iterator begin() {
    if (!m_Root) return end();
    Node<T1, T2>* p = m_Root;
    while (p->arrPtr[LCHILD]) p = p->arrPtr[LCHILD];
    return iterator(this, p);
}

iterator end() { return iterator(this, nullptr); }
```

**`operator++`** — exactly the successor algorithm above:

```cpp
// If right child exists: go right, then all the way left.
// Otherwise: climb until we came from a left-child link.
iterator& operator++() {
    if (m_Target->arrPtr[RCHILD]) {
        m_Target = m_Target->arrPtr[RCHILD];
        while (m_Target->arrPtr[LCHILD])
            m_Target = m_Target->arrPtr[LCHILD];
    } else {
        while (m_Target->arrPtr[PARENT] &&
               m_Target == m_Target->arrPtr[PARENT]->arrPtr[RCHILD])
            m_Target = m_Target->arrPtr[PARENT];
        m_Target = m_Target->arrPtr[PARENT]; // nullptr at end
    }
    return *this;
}
```

**`operator--`** is the mirror: go left then all the way right, or climb until arriving via a right-child link.

Usage:

```cpp
BST<int, float> bst;
bst.insert(MakePair(1000, 1.1f));
bst.insert(MakePair(500,  2.2f));
bst.insert(MakePair(1500, 3.3f));

for (auto iter = bst.begin(); iter != bst.end(); ++iter)
    std::cout << (*iter).first << "\n";  // prints: 500  1000  1500
```

---

## Erase

Erase is the hardest BST operation because there are three structurally distinct cases. The function takes an iterator, validates it, and returns an iterator to the in-order successor — matching the `std::map::erase` contract so callers can continue iteration safely.

```cpp
assert(_iter.m_Owner == this && _iter.m_Target != nullptr);
```

### Case 1 — Leaf node (no children)

Simply detach the node from its parent and delete it.

```cpp
if (_iter.m_Target->IsLeaf()) {
    pSuccessor = GetInorderSuccessor(_iter.m_Target);

    if (_iter.m_Target == m_Root) {
        m_Root = nullptr;
    } else {
        if (_iter.m_Target->IsLChild())
            _iter.m_Target->arrPtr[PARENT]->arrPtr[LCHILD] = nullptr;
        else
            _iter.m_Target->arrPtr[PARENT]->arrPtr[RCHILD] = nullptr;
    }

    delete _iter.m_Target;
    --m_Size;
}
```

### Case 2 — One child

Promote the single child into the deleted node's slot. The parent's pointer and the child's `PARENT` pointer both need updating.

```cpp
else if (!_iter.m_Target->IsFull()) {
    pSuccessor = GetInorderSuccessor(_iter.m_Target);

    if (_iter.m_Target == m_Root) {
        m_Root = _iter.m_Target->HasLChild()
                     ? _iter.m_Target->arrPtr[LCHILD]
                     : _iter.m_Target->arrPtr[RCHILD];
        m_Root->arrPtr[PARENT] = nullptr;
    } else {
        NODE_TYPE childSide  = _iter.m_Target->HasLChild() ? LCHILD : RCHILD;
        NODE_TYPE targetSide = _iter.m_Target->IsLChild()  ? LCHILD : RCHILD;

        // Wire parent → child and child → parent
        _iter.m_Target->arrPtr[PARENT]->arrPtr[targetSide] =
            _iter.m_Target->arrPtr[childSide];
        _iter.m_Target->arrPtr[childSide]->arrPtr[PARENT] =
            _iter.m_Target->arrPtr[PARENT];
    }

    delete _iter.m_Target;
    --m_Size;
}
```

### Case 3 — Two children (in-order successor replacement)

You cannot simply detach a node with two children without breaking the BST invariant. The standard solution:

1. Find the **in-order successor** (leftmost node of the right subtree). It is guaranteed to have **at most one child** (no left child, by definition of leftmost).
2. **Copy** the successor's key-value pair into the target node — the node address stays, only the data changes.
3. **Recursively erase** the successor, which now falls into Case 1 or Case 2.

```cpp
else {
    pSuccessor = GetInorderSuccessor(_iter.m_Target);

    // Overwrite the target's data with the successor's data
    _iter.m_Target->pair = pSuccessor->pair;

    // The successor is a leaf or has one child — erase it recursively
    iterator pNextIter(this, pSuccessor);
    erase(pNextIter);

    // The "surviving" node at this position is the target itself
    pSuccessor = _iter.m_Target;
}
```

Why this works: the in-order successor is by definition the smallest value in the right subtree, so copying it into the target preserves both constraints — it is still larger than everything in the left subtree, and still smaller than or equal to everything remaining in the right subtree. The BST invariant is unbroken.

Why the successor has at most one child: the successor is the **leftmost** node of the right subtree. If it had a left child, that child would be smaller than it and would be in the right subtree — contradicting "leftmost." So the successor either is a leaf (Case 1) or has only a right child (Case 2). This is the crucial insight that makes the recursion always terminate after one level.

Worked example — erase node 8 from `{2, 4, 8, 10, 12}`:

```
Before:          After erasing 8:
     8                10
    / \              /  \
   4   12           4    12
      /
     10
```

Step 1: In-order successor of 8 is 10 (leftmost of right subtree {12} — wait, right subtree root is 12 with left child 10, so leftmost is 10).  
Step 2: Copy 10 into node 8's slot → node now holds key 10.  
Step 3: Erase the original node holding 10 — it has no left child, so it is Case 2 (one child: 12 is promoted) or Case 1 if it were a leaf.

### Invalidating the iterator

After any of the three cases, the passed-in iterator is nulled out to prevent dangling access:

```cpp
_iter.m_Owner  = nullptr;
_iter.m_Target = nullptr;
return iterator(this, pSuccessor);
```

---

## Complexity and Balance

| Operation | Balanced | Degenerate (sorted insert) |
|-----------|----------|---------------------------|
| Search    | O(log n) | O(n)                      |
| Insert    | O(log n) | O(n)                      |
| Erase     | O(log n) | O(n)                      |

A BST degenerates into a linked list when elements are inserted in sorted order — every node becomes a right child and height reaches n. Self-balancing variants (AVL, Red-Black) bound height to O(log n) by performing rotations after insert and erase. `std::map` and `std::set` are Red-Black trees for exactly this reason.

---

## Clearing the Tree

To delete all nodes safely, use **BFS (level-order)** via a queue. This guarantees a node is not deleted before its children are enqueued — avoiding the dangling-pointer trap that a naive recursive DFS can produce if the stack unwinds in the wrong order.

```cpp
void BSTree<T1, T2>::clear() {
    if (!m_Root) return;
    std::queue<Node<T1, T2>*> q;
    q.push(m_Root);
    while (!q.empty()) {
        Node<T1, T2>* cur = q.front(); q.pop();
        if (cur->arrPtr[LCHILD]) q.push(cur->arrPtr[LCHILD]);
        if (cur->arrPtr[RCHILD]) q.push(cur->arrPtr[RCHILD]);
        delete cur;
    }
    m_Root = nullptr;
    m_Size = 0;
}
```
