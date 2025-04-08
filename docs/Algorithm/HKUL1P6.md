# Maximum Subarray Problem 

**Problem Statement:**
Given an array `A[1..n]` of integers, find a contiguous subarray (nonempty) whose sum is the largest among all possible contiguous subarrays.

**Example:**
A = [-4, 6, -3, -1, 6, 1, -2]

The maximum subarray is A[2..6] = [6, -3, -1, 6, 1], and the sum is 9.

---

**1st Attempt: Brute-force Algorithm**

**Idea:** Try all possible subarrays and compute the sum of each.

**Algorithm (Max_subarray1):**
```cpp
max = -inf;
for i = 1 to n:
    for j = i to n:
        sum = 0;
        for k = i to j:
            sum += A[k];
        if sum > max:
            max = sum;
            ans = (i, j, max);
return ans;
```
**Time Complexity:** O(n^3)

---

**Observation:**
The brute-force algorithm does a lot of redundant addition. For example:
- For `i=1, j=2`: sum = A[1] + A[2]
- For `i=1, j=3`: sum = A[1] + A[2] + A[3] (repeats the work done in `j=2`)

---

**2nd Attempt: Improved Brute-force**

**Idea:** Reuse the computed sums to avoid redundancy.

**Algorithm (Max_subarray2):**
```cpp
max = -inf;
for i = 1 to n:
    sum = 0;
    for j = i to n:
        sum += A[j];
        if sum > max:
            max = sum;
            ans = (i, j, max);
return ans;
```
**Time Complexity:** O(n^2)

This version significantly improves performance by avoiding unnecessary recomputation of subarray sums.

---

**Next Step:** Can we do even better? (Hint: use divide-and-conquer or dynamic programming)


**Maximum Subarray Problem (Divide-and-Conquer Approach)**

**Goal:** Given an array A[1..n] of integers, find a nonempty, contiguous subarray of A whose values have the largest sum.

**3rd Attempt: Divide-and-Conquer Approach**

**Key Idea:**
- Divide the array into two halves around the midpoint.
- Recursively solve the maximum subarray problem on the left half and the right half.
- Additionally, find the maximum subarray that crosses the midpoint.

### Why?
Even if the best subarray doesn't lie entirely in one half, it may span both.

**Example:**
Array: `[3, -1, 3, -5, 4, 5, -8, 2, 0, 4]`
- Left recursion returns max sum 5 for `[3, -1, 3]`
- Right recursion returns max sum 6 for `[2, 0, 4]`
- But crossing subarray `[4, 5]` yields sum 9 — the true maximum.

### Finding Maximum Subarray Across the Middle:
- Find max sum from middle to left.
```c
max = -∞; sum = 0;
for i from mid downto 1:
    sum += A[i]
    if sum > max:
        max = sum; left_index = i
```
- Find max sum from middle+1 to right.
```c
max = -∞; sum = 0;
for j from mid+1 to n:
    sum += A[j]
    if sum > max:
        max = sum; right_index = j
```
- Combine both results into a crossing subarray.
- This takes O(n) time.

### Full Recursive Algorithm:
```c
Max_subarray3(A, p, q):
  if p == q:
    return (p, q, A[p])

  mid = (p + q) // 2
  (i1, j1, max1) = Max_subarray3(A, p, mid)
  (i2, j2, max2) = Max_subarray3(A, mid + 1, q)
  (i3, j3, max3) = Max_subarray_middle(A, p, q)

  if max1 >= max2 and max1 >= max3:
    return (i1, j1, max1)
  elif max2 >= max3:
    return (i2, j2, max2)
  else:
    return (i3, j3, max3)
```

**Time Complexity:**
- T(n) = 2T(n/2) + cn  => T(n) = O(n log n)

**Can we do even better? (Hint: Yes — Kadane’s Algorithm)**


