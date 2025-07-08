# Exercises Summary

1) **Recursive Average of Array**
- **Idea**: Use a recursive function to compute the sum of the array and divide by the number of elements.
- **Algorithm**:
  ```
  avg(A[1..n]) {
    if (n == 1) return A[1];
    return (avg(A[1..n-1]) * (n-1) + A[n]) / n;
  }
  ```
- **Time Complexity**: O(n), as each recursive call processes one element.

2) **Recursive Array Reversal**
- **Idea**: Swap the first and last elements and recurse on the subarray.
- **Algorithm**:
  ```
  reverse(A, start, end) {
    if (start >= end) return;
    swap(A[start], A[end]);
    reverse(A, start + 1, end - 1);
  }
  ```
- **Time Complexity**: O(n), with only constant extra space used for recursion.

3) **Generating All n-bit Strings**
- **Idea**: Fix the last bit and recursively generate the first (n-1) bits.
- **Algorithm**:
  ```
  generate_bits(A, n) {
    if (n == 0) {
      print A;
      return;
    }
    A[n] = 0;
    generate_bits(A, n-1);
    A[n] = 1;
    generate_bits(A, n-1);
  }
  ```
- **Time Complexity**: O(2^n), since there are 2^n combinations.

4) **Generating All Subsets of a Set**
- **Idea**: Use recursion to include or exclude each element.
- **Algorithm**:
  ```
  generate_subsets(set, i, current) {
    if (i == set.length) {
      print current;
      return;
    }
    generate_subsets(set, i+1, current); // exclude
    current.add(set[i]);
    generate_subsets(set, i+1, current); // include
    current.remove(set[i]);
  }
  ```
- **Time Complexity**: O(2^n)

5) **Generating Subsets of Size <= x**
- **Idea**: Modify the subset generation to track the current size.
- **Algorithm**:
  ```
  generate_limited_subsets(set, i, current, x) {
    if (i == set.length) {
      if (current.size() <= x) print current;
      return;
    }
    generate_limited_subsets(set, i+1, current, x); // exclude
    if (current.size() < x) {
      current.add(set[i]);
      generate_limited_subsets(set, i+1, current, x); // include
      current.remove(set[i]);
    }
  }
  ```
- **Time Complexity**: O(2^n) worst-case, but may reduce depending on x.

