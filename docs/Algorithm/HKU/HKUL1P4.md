# Master Theorem, Fibonacci Example

### Master Theorem (Simplified Statement)
For recurrence of the form:
```
T(n) = aT(n/b) + f(n)
```
where:
- `a ≥ 1` (number of subproblems)
- `b ≥ 2` (factor of size reduction)
- `f(n) = Θ(n^p)` (non-recursive work per level)

Then:
- If `f(n) = O(n^{log_b a - ε})` for some ε > 0:
    - **T(n) = Θ(n^{log_b a})**
- If `f(n) = Θ(n^{log_b a})`:
    - **T(n) = Θ(n^{log_b a} log n)**
- If `f(n) = Ω(n^{log_b a + ε})` and regularity condition holds:
    - **T(n) = Θ(f(n))**

> For cases where `n` is not a perfect power of `b`, the time complexity usually stays the same, but algorithms may require additional handling for leftover elements.

---

### Fibonacci Numbers: Recursive vs Iterative

#### Definition:
```
F(1) = 1
F(2) = 1
F(n) = F(n-1) + F(n-2) for n > 2
```

---

### Algorithm 1: Naive Recursion
```cpp
FIB1(n):
  if n ≤ 2:
    return 1
  else:
    return FIB1(n-1) + FIB1(n-2)
```
- **Recurrence:**
    - T(n) = T(n-1) + T(n-2) + c₂
    - T(n) = exponential time (approximately Θ(1.618^n))
- **Inefficient**: Recomputes many values

**Claim:** T(n) > 2^(n/2 - 1)
- Recursive calls double roughly every two levels
- Exponential growth due to repeated calls

---

### Algorithm 2: Iterative Loop
```cpp
FIB2(n):
  if n ≤ 2:
    return 1
  p = 1
  q = 1
  for i = 3 to n:
    r = p + q
    p = q
    q = r
  return r
```
- **Time Complexity:** Θ(n)
- **Efficient**: No redundant computations

---

### Q: Can We Do Even Better?
- Yes! Matrix exponentiation or using Binet’s formula (closed-form) can bring time complexity to **O(log n)**.
- Example: Fast doubling or matrix power via divide-and-conquer.

