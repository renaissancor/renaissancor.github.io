# Parametric Search

## 1. Concept Overview

### Binary Search

An efficient algorithm to find a target value in a **sorted** array.
It repeatedly halves the search range until the target is found or the range becomes empty.

```cpp
int binary_search(vector<int>& arr, int target) {
    int lo = 0, hi = arr.size() - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}
```

**Time complexity:** `O(log N)`

---

### Parametric Search

Instead of searching for an exact value, we binary search the **answer space** to find the smallest or largest value that satisfies a condition.

It is used when:

* We can check if a given value `x` is feasible (`possible(x)`).
* The feasibility is **monotonic** (if `x` works, all larger or smaller values also work).

---

## 2. General Template

```cpp
bool possible(int mid) {
    // Return true if mid satisfies the condition
}

int parametric_search() {
    int lo = MIN, hi = MAX, ans = hi;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (possible(mid)) {
            ans = mid;
            hi = mid - 1;  // try to find a smaller valid value
        } else {
            lo = mid + 1;  // increase the value
        }
    }
    return ans;
}
```

---

## 3. Example 1 – Split Array Minimization

### Problem

Given an array `a` of size `n`, divide it into `m` **consecutive groups**
so that the **maximum group sum** is minimized.

### Idea

1. Guess a possible maximum group sum `x`.
2. Check if the array can be split into ≤ `m` groups such that each group’s sum ≤ `x`.
3. Binary search for the smallest valid `x`.

### Implementation

```cpp
bool possible(const int x) {
    int group_sum = 0, group_cnt = 1;
    for (int val : a) {
        if (group_sum + val > x) {
            group_sum = val;
            group_cnt++;
        } else {
            group_sum += val;
        }
    }
    return group_cnt <= m;
}

int main() {
    cin >> n >> m;
    a.resize(n);
    for (int &v : a) cin >> v;

    int lo = *max_element(a.begin(), a.end());
    int hi = accumulate(a.begin(), a.end(), 0);

    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (possible(mid)) hi = mid - 1;
        else lo = mid + 1;
    }

    cout << lo << '\n';
}
```

**Time complexity:** `O(N log S)`
where `S` is the value range (`hi - lo`).

---

## 4. Example 2 – Longest Increasing Subsequence (LIS)

### Idea

Use binary search to maintain the smallest possible tail value for each subsequence length.

```cpp
vector<ll> sub;
sub.push_back(a[0]);
for (size_t i = 1; i < n; ++i) {
    auto it = lower_bound(sub.begin(), sub.end(), a[i]);
    if (it == sub.end()) sub.push_back(a[i]);
    else *it = a[i];
}
cout << sub.size() << '\n';
```

**Time complexity:** `O(N log N)`

---

## 5. Summary Table

| Type                    | Description                             | Time Complexity |
| ----------------------- | --------------------------------------- | --------------- |
| Binary Search           | Find a target in a sorted array         | `O(log N)`      |
| Parametric Search       | Binary search over the answer space     | `O(N log S)`    |
| LIS (via Binary Search) | Find the longest increasing subsequence | `O(N log N)`    |

---

## 6. Key Takeaways

* The condition in parametric search must be **monotonic**.
* The search range usually represents **values**, not **indices**.
* The `possible()` function defines the feasibility check for a given parameter.
* Common use cases include:

  * Minimizing the largest group sum
  * Cutting cables or wood at optimal length
  * Scheduling problems
  * Maximum minimum distance problems
