# Divide and Conquer

## Learning Outcome
- Learn how to analyze recursive algorithms.
- Understand the divide-and-conquer technique, recurrence relations, and recursive functions.

## What is a Recursive Function?
- A recursive function calls itself with a smaller input.
- It consists of:
    - A base case that stops the recursion.
    - A recursive case that breaks the problem into a smaller subproblem.

### Example
```
f(n) = 1                 if n = 1
f(n) = f(n - 1) + n      if n > 1
```
To compute f(4):
```
f(4) = f(3) + 4
     = f(2) + 3 + 4
     = f(1) + 2 + 7
     = 1 + 9 = 10
```

## Finding a Closed Form (Non-Recursive Formula)

### Method: Iteration
By expanding the recurrence:
```
f(n) = f(n-1) + n
     = f(n-2) + (n-1) + n
     = ...
     = 1 + 2 + ... + n = n(n+1)/2
```

### Mathematical Induction
To prove: f(n) = n(n+1)/2
- Base case: n = 1 → f(1) = 1, and 1(1+1)/2 = 1 ✅
- Inductive step:
  Assume f(k) = k(k+1)/2
  Show f(k+1) = f(k) + (k+1) = k(k+1)/2 + (k+1) = (k+1)(k+2)/2 ✅

## Asymptotic Analysis (Big-O)
We often care about the growth rate instead of the exact formula.

### Prove: f(n) = O(n²)
We want to find a constant c such that f(n) ≤ cn² for all n ≥ 1.

Step 1: Trial to find c
```
f(n) = f(n-1) + n ≤ c(n-1)² + n = cn² - (2cn - c - n)
```
To ensure f(n) ≤ cn², we need:
```
2cn - c - n ≥ 0 → c ≥ n / (2n - 1)
```
Since n / (2n - 1) ≤ 1 for all n ≥ 1, choose c = 1.

Step 2: Induction proof
- Base: f(1) = 1 ≤ 1²
- Inductive step:
  Assume f(k) ≤ k²
  Then f(k+1) = f(k) + (k+1) ≤ k² + (k+1) ≤ (k+1)²

So, f(n) = O(n²) is proven.

## Take-home Exercise
Prove: f(n) = O(n), where
```
f(1) = 3
f(n) = f(n - 1) + 10 for n > 1
```

## Solving Recurrences (How to Find Asymptotic Bounds)
Here are three common techniques:
1. Substitution Method
2. Recursion Tree Method
3. Master Method

These are detailed in MIT Introduction to Algorithms (CLRS), Chapters 4.3 to 4.5.
While not a major exam topic, understanding them is useful for mastering algorithm analysis. You can review online materials or request a tutorial if needed.

## Divide and Conquer Overview
This approach leads to recursive algorithms. Their time complexity is usually described by recurrence relations.

The general structure:
1. Divide the problem into subproblems
2. Conquer each subproblem recursively
3. Combine their solutions

## Factorial Example
### Recursive Code
```cpp
fact(n) {
  if (n == 1) return 1;
  else return n * fact(n-1);
}
```

### Time Complexity Analysis
Let T(n) be the runtime:
- T(1) = c1
- T(n) = T(n - 1) + c2 (for n > 1)

Unfolding:
```
T(n) = T(n-1) + c2
     = T(n-2) + 2c2
     = ...
     = T(1) + (n - 1)c2 = c1 + (n - 1)c2
```
Therefore, T(n) = O(n)

This shows that the recursive factorial algorithm runs in linear time.

# Tower of Hanoi - Summary

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

This version includes structured visuals and step-by-step logic for explaining Tower of Hanoi to students in an intuitive way. You can draw each state on a whiteboard or use the pseudocode and recurrence to connect with algorithm analysis.

