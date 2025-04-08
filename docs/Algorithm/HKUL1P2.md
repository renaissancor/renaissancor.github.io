# Tower of Hanoi

## Problem Overview
- Given **3 pegs** and **n disks** of different sizes stacked in increasing size on one peg (largest at bottom).
- Goal: Move all disks from the source peg to the destination peg.

## Rules
1. Only one disk can be moved at a time.
2. A disk can only be placed on top of a larger disk or an empty peg.

---

## Recursive Solution Strategy
To move `n` disks from Peg A to Peg C using Peg B:
1. Move `n-1` disks from A to B (using C as auxiliary).
2. Move the largest disk (nth) from A to C.
3. Move the `n-1` disks from B to C (using A as auxiliary).

### Base Case
If `n = 1`: Move the single disk directly from A to C.

### Pseudocode
```cpp
TofH(A, B, C, n) {
    if (n == 1)
        move disk from A to C;
    else {
        TofH(A, C, B, n - 1);
        move disk from A to C;
        TofH(B, A, C, n - 1);
    }
}
```

---

## Visual Explanation for n = 3

### Initial State
```
Peg A       Peg B       Peg C
[3]         |           |
[2]         |           |
[1]         |           |
```

### Step-by-step Moves
1. Move 2 disks from A to B (use C):
    - Move [1] A → C
    - Move [2] A → B
    - Move [1] C → B
```
Peg A       Peg B       Peg C
[3]         [2]         |
            [1]         |
```

2. Move [3] A → C
```
Peg A       Peg B       Peg C
            [2]        [3]
            [1]         |
```

3. Move 2 disks from B to C (use A):
    - Move [1] B → A
    - Move [2] B → C
    - Move [1] A → C
```
Peg A       Peg B       Peg C
            |          [3]
            |          [2]
            |          [1]
```

---

## Number of Moves
Let f(n) be the number of moves:
- f(1) = 1
- f(n) = 2f(n-1) + 1

Solving recurrence:
```
f(n) = 2f(n - 1) + 1
     = 2(2f(n - 2) + 1) + 1 = 4f(n - 2) + 3
     = ...
     = 2ⁿ - 1
```

## Time Complexity
- Recursive algorithm performs `2ⁿ - 1` moves
- ⇒ Time complexity: **O(2ⁿ)** (exponential)

---


## ⚠️ Incorrect Attempt at Hanoi
**Student’s Wrong Approach**:
```cpp
TofH2(A, B, C, n) {
  if (n == 1)
    move disk from A to C;
  else {
    TofH2(A, C, B, n/2);
    TofH2(A, B, C, n/2);
    TofH2(B, A, C, n/2);
  }
}
```

### Why It’s Incorrect
- Tower of Hanoi must preserve **disk ordering**.
- Splitting into `n/2` is invalid because it breaks the top-to-bottom size rule.
- The correct subproblem is always: move top `n-1` disks first.

---

