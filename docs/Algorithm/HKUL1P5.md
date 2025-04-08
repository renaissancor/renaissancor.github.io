# Fibonacci Numbers & Matrix Exponentiation Summary 

---

### Problem: Compute the n-th Fibonacci number efficiently

We’ve seen two common approaches:

1. **Recursive (FIB1)**
    - Direct implementation of the recurrence.
    - Time Complexity: Exponential, \( T(n) = \Theta(1.618^n) \)
    - Inefficient due to repeated computation.

2. **Iterative (FIB2)**
    - Uses a loop to iteratively build up to \( F_n \).
    - Time Complexity: \( \Theta(n) \)
    - Much faster, but can we go even faster?

---

### Matrix Exponentiation

Fibonacci numbers can be computed using matrix exponentiation:

Let:
```
    | 0 1 |
A = | 1 1 |
```
Then,
```
    | F(n)   F(n+1) |
Aⁿ =| F(n-1) F(n)   |
```

To compute Fibonacci numbers, compute \( A^{n-1} \), and return the value at position (2,2).

---

### Matrix Power Algorithms

**1st Attempt: Straightforward Recursion**
```cpp
Power1(A, n):
  if (n == 1): return A
  else: return Multiply(A, Power1(A, n-1))
```
- Time: \( O(n) \)

**2nd Attempt: If n is a power of 2**
```cpp
Power2(A, n):
  if (n == 1): return A
  else:
    B = Power2(A, n/2)
    return Multiply(B, B)
```
- Time: \( O(\log n) \)

**3rd Attempt: General n (Odd or Even)**
```cpp
Power3(A, n):
  if (n == 1): return A
  if (n is even):
    B = Power3(A, n/2)
    return Multiply(B, B)
  else:
    B = Power3(A, (n-1)/2)
    return Multiply(A, Multiply(B, B))
```
- Time: \( O(\log n) \)

---

### Application to Fibonacci
- Use the matrix:
```
    | 0 1 |
    | 1 1 |
```
- Compute \( A^{n-1} \) using Power3 to get \( F(n) \) efficiently.
- Time Complexity: \( O(\log n) \), very efficient for large n.


