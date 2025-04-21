**Summary & Exam Guide: Recursion and Divide-and-Conquer**

---

### ðŸ“– **Core Concepts Covered**

#### âœ… 1. Divide and Conquer
- Problem is split into subproblems, solved recursively, and solutions combined.
- Common recurrence: `T(n) = 2T(n/2) + cn`
- Time complexity often solved via **Master Theorem**:
    - If `f(n) = Î˜(n^p)`, then:
        - If `a > b^p` âž” `T(n) = Î˜(n^log_b a)`
        - If `a = b^p` âž” `T(n) = Î˜(n^p log n)`
        - If `a < b^p` âž” `T(n) = Î˜(n^p)`

#### âœ… 2. Recursion
- Recursive algorithms must define:
    - Base case
    - Recursive step
- Should always aim to reduce the problem size.

#### âœ… 3. Recurrence and Complexity
- Learn to express recursive time complexity and simplify using:
    - Recursion Tree
    - Master Theorem

---

### ðŸ”§ **Classic Recursive Problems**

#### â€¢ Largest Number in Array
- Divide array into halves.
- Base: single element
- Combine: return max of two halves
- Time Complexity: O(n)

#### â€¢ Fibonacci Numbers
- Naive recursion: exponential time `T(n) = T(n-1) + T(n-2)`
- Iterative: O(n)
- Matrix Exponentiation: O(log n)

#### â€¢ Maximum Subarray Problem
- Brute-force: O(n^3)
- Improved brute-force: O(n^2)
- Divide-and-conquer: O(n log n)
- (Optional) Kadaneâ€™s algorithm: O(n)

#### â€¢ Matrix Exponentiation
- Naive: O(n)
- Recursive squaring: O(log n)

---

### âœ¨ **Key Implementation Exercises**

1. **Average of Array**: Use recursion to divide and compute average.
2. **Reverse Array**: Swap elements recursively with constant space.
3. **Generate Binary Strings**: Fix last bit recursively and backtrack.
4. **Generate Subsets**: Include/exclude current element recursively.

---

### ðŸ“š **Exam Tips**

- Know how to **write recursive functions** with correct base and recursive cases.
- Be able to **analyze time complexity** using recurrence relations.
- Understand where recursion is inefficient and should be replaced with iteration.
- Trace recursive calls and predict outputs.

---

### ðŸ”¹ **Practice Questions You Might See**
- Write a recursive algorithm to compute the average of numbers.
- Implement and analyze recursive Fibonacci.
- Use divide-and-conquer to solve max subarray problem.
- Derive time complexity from a given recursive function.
- Generate all subsets of a set recursively.


---

### ðŸ”¤ **Asymptotic Notation Key**

| Symbol | Name           | Meaning                                                                                  |
|--------|----------------|------------------------------------------------------------------------------------------|
| **O**  | Big-O          | **Upper bound**: f(n) grows at **most as fast** as g(n), up to constant factors          |
| **Î©**  | Big-Omega      | **Lower bound**: f(n) grows at **least as fast** as g(n), up to constant factors         |
| **Î˜**  | Big-Theta      | **Tight bound**: f(n) grows at **the same rate** as g(n), up to constant factors         |
| **o**  | Little-o       | **Strictly slower**: f(n) grows **slower** than g(n); f(n)/g(n) â†’ 0 as n â†’ âˆž             |
| **Ï‰**  | Little-omega   | **Strictly faster**: f(n) grows **faster** than g(n); f(n)/g(n) â†’ âˆž as n â†’ âˆž             |

---

### ðŸ’¡ Intuition Cheat Sheet

- `f(n) = O(g(n))` â†’ f **â‰¤** g
- `f(n) = Î©(g(n))` â†’ f **â‰¥** g
- `f(n) = Î˜(g(n))` â†’ f **â‰ˆ** g
- `f(n) = o(g(n))` â†’ f **â‰ª** g
- `f(n) = Ï‰(g(n))` â†’ f **â‰«** g

---

Would you like me to walk through that table now and fill in the T/F for each row based on these?



