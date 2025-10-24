# Union Find Algorithm 

Basic Idea 

However, in worst case, time complexity to find 
root might be close to `O(n)` without optimization 

## Optimization  I : Union By Rank 

Without optimization, saving root as itself is fine. 
However, saving Root Node as negative integer like `-1` 
is sometimes required. 

Let root node save their depth in negative value. 
Then, when making union function is required, do follow. 

```cpp
void make_union(int u, int v) {
        u = find(u); 
        v = find(v); 
        if(u == v) return; // In Same Union 
        else if (data[u] > data[v]) data[v] = u; 
        else if (data[u] < data[v]) data[u] = v; 
        else {
            data[v] = u; 
            data[u]--; 
        }
    }
```

## Optimization II : Path Compression 

During find process, let all children point same root, 
so that time complexity to find root will be `O(1)` next time. 

Easiest Implementation is recursion. 

```cpp
int find(int x) { // Return root index, not data[x] 
    if(data[x] < 0) return x; 
    return data[x] = find(data[x]); 
}
```

More clearly, tracking by iteration and stack is possible. 
Then, making it as inline function is also available. 

```cpp
inline int find(int x) { // Return root index, not data[x] 
    if(data[x] < 0) return x; 
    vector<int> stk; 
    while(data[x] >= 0) {
        stk.push_back(x); 
        x = data[x]; 
    }
    for (int &i : stk) data[i] = x; 
    return x; 
}
```

Full Code 

```cpp
class cunion {
private:
    vector<int> data;
public:
    cunion(int n) : data(n, -1) {}
    int find(int x) noexcept { // Return root index, not data[x] 
        if(data[x] < 0) return x; 
        return data[x] = find(data[x]); 
    }
    inline bool is_union(int u, int v) noexcept {
        return find(u) == find(v);
    }
    inline void make_union(int u, int v) noexcept {
        u = find(u), v = find(v); 
        if(u == v) return; // In Same Union 
        else if (data[u] > data[v]) data[v] = u; 
        else if (data[u] < data[v]) data[u] = v; 
        else {
            data[v] = u; 
            data[u]--; 
        }
    }
}; 
```
