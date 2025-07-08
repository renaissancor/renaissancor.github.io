**Mathematical Induction Proofs and Fibonacci Algorithms**

---

### 1. Sum of First n Natural Numbers

We want to prove:
\[ f(n) = 1 + 2 + 3 + \dots + n = \frac{n(n + 1)}{2} \]

#### Case 1: Base Case (n = 1)
\[ f(1) = 1 = \frac{1(1 + 1)}{2} = \frac{2}{2} = 1 \quad \text{True} \]

#### Case 2: Inductive Step
**Assume** the formula works for some \( k \):
\[ f(k) = \frac{k(k + 1)}{2} \]

We want to prove it works for \( k + 1 \):
\[ f(k + 1) = f(k) + (k + 1) \]
\[ = \frac{k(k + 1)}{2} + (k + 1) \]
\[ = \frac{k(k + 1) + 2(k + 1)}{2} \]
\[ = \frac{(k + 1)(k + 2)}{2} \quad \text{True} \]

Conclusion: By induction, the formula holds for all \( n \in \mathbb{N} \).

---

### 2. Sum of Squares
We want to prove:
\[ \sum_{i=1}^n i^2 = \frac{n(n + 1)(2n + 1)}{6} \]

#### Case 1: Base Case (n = 1)
\[ \frac{1 \cdot 2 \cdot 3}{6} = 1 \quad \text{True} \]

#### Case 2: Inductive Step
Assume it works for some \( k \):
\[ \sum_{i=1}^k i^2 = \frac{k(k + 1)(2k + 1)}{6} \]

Show it holds for \( k + 1 \):
\[ \sum_{i=1}^{k+1} i^2 = \sum_{i=1}^k i^2 + (k + 1)^2 \]
\[ = \frac{k(k + 1)(2k + 1)}{6} + (k + 1)^2 \]

Combine:
\[ = \frac{k(k + 1)(2k + 1) + 6(k + 1)^2}{6} \]
\[ = \frac{(k + 1)(k(2k + 1) + 6(k + 1))}{6} \]
\[ = \frac{(k + 1)(k + 2)(2k + 3)}{6} \quad \text{True} \]

Conclusion: Formula is valid by induction.

---

### 3. Fibonacci Implementations

#### Recursive Version
```cpp
FIB1(n):
  if n ≤ 2:
    return 1
  else:
    return FIB1(n - 1) + FIB1(n - 2)
```
- Time complexity: \( O(2^n) \) — very inefficient for large n.

#### Dynamic Programming Version
```cpp
FIB_ARRAY[n] = [0];
vector<int> fib_array(n);
fib_array.resize(n);

FIBDP(n):
  if n ≤ 2:
    return 1
  else if FIB_ARRAY[n] == 0:
    FIB_ARRAY[n] = FIBDP(n - 1) + FIBDP(n - 2)
  return FIB_ARRAY[n];
```
- Time complexity: \( O(n) \)
- Uses memoization to avoid recomputation.

---

### Summary
- Induction proofs work by validating the base case and extending to \( k + 1 \) from \( k \).
- The sum of natural numbers and sum of squares are classic examples.
- Fibonacci recursion is elegant but inefficient; dynamic programming makes it scalable.



