# Divide-and-Conquer Approach 

## Key Concepts
1. **Divide-and-Conquer Strategy**:
    - Solve a problem of size `n` by assuming solutions exist for smaller inputs (`n-1`, or `n/2`).
    - Then, combine those solutions to build the solution for size `n`.

2. **Base Case is Essential**:
    - For factorial: `n! = 1` when `n = 1`
    - For Tower of Hanoi: Move 1 disk directly

3. **Steps in Divide-and-Conquer**:
    - Define subproblems
    - Solve base cases
    - Combine subproblem solutions
    - Formulate a recursive algorithm
    - Model running time as recurrence and solve it

---

## Example: Finding the Maximum Number

### Goal
Find the largest value in an array using recursion.

### Strategy
- Base Case: If one number, return it.
- Recursive Case: Compare the last number with the result from a recursive call on the rest.

### Pseudocode
```cpp
largest1(A[1..n]) {
  if (n == 1) return A[1];
  x = largest1(A[1..n-1]);
  if (x > A[n]) return x;
  else return A[n];
}
```

### Time Complexity
- `T(n) = T(n-1) + c2`, `T(1) = c1`
- ⇒ **T(n) = O(n)**

### Example Execution
```
largest1([4, 60, 21, 3, 5])
  → largest1([4, 60, 21, 3])
    → largest1([4, 60, 21])
      → largest1([4, 60])
        → largest1([4]) → 4
        → return max(4, 60) → 60
      → return max(60, 21) → 60
    → return max(60, 3) → 60
  → return max(60, 5) → 60
```

---


### Divide and Conquer: Finding the Largest Number (Alternative Approach)

#### Problem:
Find the largest number in an array of size `n` using divide-and-conquer.

---

### Key Idea:
- **Divide** the array into two equal halves.
- **Conquer** each half recursively.
- **Combine**: Compare the maximums from both halves.

---

### Algorithm:
```c
// Assume n is a power of 2 for simplicity
largest2(A[1..n]) {
    if (n == 1)
        return A[1];
    else {
        x1 = largest2(A[1..n/2]);
        x2 = largest2(A[n/2+1..n]);
        if (x1 > x2)
            return x1;
        else
            return x2;
    }
}
```

### Time Complexity:
Let T(n) be the time taken to find the largest element in n numbers.
- T(1) = c1
- T(n) = 2T(n/2) + c2
- **Using Master Theorem**, this gives **T(n) = O(n)**

---

### Example:
Array: [14, 65, 34, 33, 7, 56, 100, 20]

Step-by-step recursive split:
```
Left: [14, 65, 34, 33]           Right: [7, 56, 100, 20]
  ├── [14, 65]                      ├── [7, 56]
  │     ├── 14                     │     ├── 7
  │     └── 65                     │     └── 56
  └── [34, 33]                     └── [100, 20]
        ├── 34                           ├── 100
        └── 33                           └── 20
```
Compare and propagate max values up:
- max(14, 65) → 65
- max(34, 33) → 34
- max(65, 34) → 65
- max(7, 56) → 56
- max(100, 20) → 100
- max(56, 100) → 100
- final max: max(65, 100) → **100**

---

### Comparison with Straightforward Approach:
```c
largest(A[1..n]) {
    maximum = A[1];
    for (i = 2; i <= n; i++) {
        if (A[i] > maximum)
            maximum = A[i];
    }
    return maximum;
}
```
- **Also O(n)** time complexity.
- This method is simpler, but the divide-and-conquer version helps practice recursion.

---

### Summary:
- Divide-and-conquer is not always the most efficient in practice but is helpful conceptually.
- Good for illustrating recursion and recurrence analysis.
