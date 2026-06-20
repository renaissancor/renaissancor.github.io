---
title: "Trees and Traversals"
tags:
  - data-structures
  - trees
  - traversal
  - bst
---

# Trees and Traversals

Trees are hierarchical data structures where nodes connect by edges and no cycles exist. Every node except the root has exactly one parent; the root has none.

## Core Terminology

| Term | Definition |
|------|-----------|
| **Root** | Topmost node; has no parent |
| **Leaf** | Node with no children |
| **Internal node** | Node with at least one child |
| **Depth** | Edge-count from root to a given node |
| **Height** | Edge-count on the longest path from a node down to a leaf |
| **Degree** | Number of children a node has |

## Binary Tree Variants

A **binary tree** restricts each node to at most two children (left, right).

```
   A
  / \
 B   C
/ \ / \
D E F  G
```

**Complete binary tree (CBT)** — levels fill left-to-right; every level except possibly the last is full, and the last level's nodes sit as far left as possible.

```
    1
   / \
  2   3
 / \
4   5
```

**Perfect binary tree** — a CBT where every level is completely filled; all leaves share the same depth. A perfect tree is a *state* of a complete tree, not a separate structure.

```
    1
   / \
  2   3
 / \ / \
4  5 6  7
```

## Binary Search Trees

A BST enforces an ordering invariant: every value in a node's **left** subtree is strictly less than the node's value; every value in its **right** subtree is strictly greater.

```
    8
   / \
  3   10
 / \    \
1   6    14
   / \   /
  4   7 13
```

This invariant makes each comparison eliminate an entire subtree, yielding O(log n) average-case performance — but only when the tree stays balanced.

| Operation | Average | Worst (degenerate) |
|-----------|---------|-------------------|
| Search | O(log n) | O(n) |
| Insert | O(log n) | O(n) |
| Delete | O(log n) | O(n) |

Inserting already-sorted data (1, 2, 3, 4, 5…) reduces a BST to a linked list — the worst case. Self-balancing variants (AVL, Red-Black) prevent this by rebalancing after each mutation.

How does BST compare to the structures you already know?

| Structure | Insert | Indexing by position | Delete | Search |
|-----------|--------|---------------------|--------|--------|
| Dynamic array | O(1) amortised | O(1) | O(n) | O(n) |
| Linked list | O(1) | O(n) | O(1) | O(n) |
| BST (balanced) | O(log n) | O(n) | O(log n) | O(log n) |

BST trades O(1) random-access indexing for O(log n) search and delete — a worthwhile deal when you need ordered, searchable data and don't need positional access. The C++ standard library exposes this via `std::set` and `std::map`, both typically implemented as Red-Black trees.

## Tree Traversals

Traversal = visit every node exactly once. The order determines what you get.

### Recursive depth-first traversals

All three share the same recursive skeleton; only the position of the "visit" step changes.

```python
def preorder(node):      # Node → Left → Right
    if node is None:
        return
    visit(node)
    preorder(node.left)
    preorder(node.right)

def inorder(node):       # Left → Node → Right
    if node is None:
        return
    inorder(node.left)
    visit(node)
    inorder(node.right)

def postorder(node):     # Left → Right → Node
    if node is None:
        return
    postorder(node.left)
    postorder(node.right)
    visit(node)
```

- **Preorder** processes a node before its subtrees — useful for copying a tree or serialising it.
- **Inorder** on a BST visits nodes in sorted ascending order — the foundation of BST iterators (`std::set`, `std::map`).
- **Postorder** processes children before their parent — the natural order for freeing memory or evaluating postfix expressions.

### Level-order (BFS)

Visit nodes level by level, top to bottom. Requires an explicit queue because the recursive call stack follows a single path, not a broad front.

```python
from collections import deque

def level_order(root):
    if root is None:
        return
    queue = deque([root])
    while queue:
        node = queue.popleft()
        visit(node)
        if node.left:
            queue.append(node.left)
        if node.right:
            queue.append(node.right)
```

### Worked example

```
Given:
    A
   / \
  B   C
 / \ /
D  E F

Preorder:    A  B  D  E  C  F
Inorder:     D  B  E  A  F  C
Postorder:   D  E  B  F  C  A
Level-order: A  B  C  D  E  F
```

Trace inorder mentally: go left as far as possible (D), backtrack to B, visit E, backtrack to A, then descend into C's left child F before finally visiting C. The left-subtree-first rule fully unwinds before the parent is emitted.

Postorder reversal check: D and E are both children of B, so they appear before B. F is a child of C, so F appears before C. B and C are children of A, so A is last.

All four orders are O(n) — every node is visited exactly once.

### In-order successor in a BST

A useful BST primitive: given a node, find the next-larger value (its in-order successor).

- **Node has a right child**: the successor is the **leftmost node** in the right subtree — keep following `.left` until there is no left child.
- **Node has no right child**: walk up the ancestor chain and return the first ancestor for which the current node is in its *left* subtree (i.e., the lowest ancestor whose left child is also an ancestor of the current node).

This comes up in delete-with-replacement and in implementing BST iterators.

### Recursive vs. iterative

Recursive traversals are concise but consume O(h) call-stack frames (h = tree height). On a degenerate BST h = n, risking a stack overflow for large inputs. An iterative preorder, for example, uses an explicit stack instead:

```python
def preorder_iterative(root):
    if root is None:
        return
    stack = [root]
    while stack:
        node = stack.pop()
        visit(node)
        if node.right:          # push right first so left is processed first
            stack.append(node.right)
        if node.left:
            stack.append(node.left)
```

The call-stack frames become heap-allocated stack entries — the traversal order is identical, and memory exhaustion from deep recursion is no longer a concern.

## Heap

A heap is a complete binary tree that maintains the **heap property**:

- **Min-heap**: every parent ≤ its children (root holds the minimum)
- **Max-heap**: every parent ≥ its children (root holds the maximum)

Because a CBT's shape is fully determined by node count, it maps directly to an array without pointers:

```
Index k  →  left child: 2k+1  |  right child: 2k+2  |  parent: (k-1)//2

Max-heap array: [10, 7, 9, 3, 2, 8, 1]

     10
    /  \
   7    9
  / \  / \
 3  2 8   1
```

Key operations:

| Operation | Time |
|-----------|------|
| Find max/min | O(1) — always at index 0 |
| Insert | O(log n) — append then sift up |
| Extract max/min | O(log n) — swap root with last, remove last, sift down |
| Build heap from n unsorted elements | O(n) |

**Heap sort** uses this: build a max-heap in O(n), then repeatedly extract the max in O(log n), yielding a sorted array in O(n log n) total. **Priority queues** expose only insert and extract-max/min, making the heap the natural backing structure.

## Self-Balancing BSTs

When a plain BST degenerates, all operations fall to O(n). Balancing trees rebalance automatically on mutation.

| Variant | Balance criterion | Rotations on insert/delete |
|---------|------------------|-----------------------------|
| **AVL tree** | Height difference between siblings ≤ 1 | More frequent — stricter balance |
| **Red-Black tree** | Node colouring rules keep longest path ≤ 2× shortest | Fewer — more relaxed balance |

AVL trees give slightly faster lookups because the height difference between any two siblings is at most 1 — the strictest possible balance. Red-Black trees tolerate a looser invariant (longest path ≤ 2× shortest), which means fewer rotations per write. For read-heavy workloads, AVL is marginally faster; for write-heavy workloads, Red-Black wins. `std::map` and `std::set` in the C++ standard library use Red-Black trees because real-world workloads are typically write-heavy enough to favour the lower rotation cost.
