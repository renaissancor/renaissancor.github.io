
- \( f(n) = O(g(n)) \) – “Big-O”: f grows **no faster** than g (upper bound).
- \( f(n) = \Omega(g(n)) \) – “Big-Omega”: f grows **at least as fast** as g (lower bound).
- \( f(n) = \Theta(g(n)) \) – “Big-Theta”: f and g grow at the **same rate**.
- \( f(n) = o(g(n)) \) – “little-o”: f grows **strictly slower** than g.
- \( f(n) = \omega(g(n)) \) – “little-omega”: f grows **strictly faster** than g.

---

### Let's fill out the table for each pair:

| **f(n)**            | **g(n)**            | **O** | **Ω** | **Θ** | **o** | **ω** | **Explanation**                             |
|---------------------|---------------------|-------|-------|-------|-------|-------|---------------------------------------------|
| \( n^2 \)           | \( n^3 \)           | T     | F     | F     | T     | F     | \( n^2 < n^3 \), strictly slower growth     |
| \( 2^n \)           | \( \log \sqrt{n} \) | F     | T     | F     | F     | T     | Exponential >> any log variant              |
| \( \sqrt{\log n} \) | \( 3n \)            | T     | F     | F     | T     | F     | Log grows much slower than linear           |
| \( \log^2 n \)      | \( \log_{10}n \)    | F     | T     | F     | F     | T     | \( \log^2 n = (\log n)^2 \) >> \( \log n \) |
| \( n \cos n \)      | \( \sqrt{3} \)      | F     | T     | F     | F     | T     | Oscillates but grows unbounded vs constant  |
| \( \sqrt{n} \)      | \( n \)             | T     | F     | F     | T     | F     | Root is slower than linear                  |

---

### Final Completed Table:

| **f(n)**            | **g(n)**            | **O** | **Ω** | **Θ** | **o** | **ω** |
|---------------------|---------------------|-------|-------|-------|-------|-------|
| \( n^2 \)           | \( n^3 \)           | T     | F     | F     | T     | F     |
| \( 2^n \)           | \( \log \sqrt{n} \) | F     | T     | F     | F     | T     |
| \( \sqrt{\log n} \) | \( 3n \)            | T     | F     | F     | T     | F     |
| \( \log^2 n \)      | \( \log_{10}n \)    | F     | T     | F     | F     | T     |
| \( n \cos n \)      | \( \sqrt{3} \)      | F     | T     | F     | F     | T     |
| \( \sqrt{n} \)      | \( n \)             | T     | F     | F     | T     | F     |

---
