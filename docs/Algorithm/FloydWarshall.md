# Floyd-Warshall Algorithm 

## BFS Breadth First Search implementation 

BOJ4179 

Save when fire reaches first by BFS.  

```cpp
for (int cnt = 0; !bfs.empty(); ++cnt) {
    size_t qcnt = bfs.size(); 
    for(size_t qi = 0; qi < qcnt; ++qi) {
        Node node = bfs.front(); bfs.pop(); 
        g[node.r][node.c] = cnt; 
        for(const Node &dir : dirs) {
            Node next = node + dir; 
            if(out_range(next)) continue; 
            if(g[next.r][next.c] != INT_MAX) continue; // Enqueued  
            g[next.r][next.c] = -2; 
            bfs.push(next); 
        }
    }
}
```

Now, simulate all available path J can go by bFS
```cpp

int func() {
    bool success = false; 
    int cnt = 0; 
    queue<Node> bfs; 

    bfs.push({jr, jc}); 
    vector<vector<bool>> visit(row, vector<bool>(col, false)); 
    visit[jr][jc] = true; 

    int ans = INT_MAX; 
    for (cnt = 1; !bfs.empty(); ++cnt) {
        size_t qcnt = bfs.size(); 
        for(size_t qi = 0; qi < qcnt; ++qi) {
            Node node = bfs.front(); bfs.pop(); 
            for(const Node &dir : dirs) {
                Node next = node + dir; 
                if(out_range(next)) {
                    ans = min(ans, cnt); 
                    break; 
                } 
                else if(visit[next.r][next.c]) continue; 
                else if(g[next.r][next.c] < 0) continue; 
                else if(cnt >= g[next.r][next.c]) continue; // In Fire 
                else {
                    bfs.push(next); 
                    visit[next.r][next.c] = true; 
                }
            }
        }
    }
    return ans; 
}
```



## Floyd-Warshall Algorithm 

Graph based on Adjacent Matrix only. 

Time Complexity $O(V^3)$ 

Recommended when edges are many while vertex is relatively 
smaller to make $V^3 \leq {10}^8$ 

```cpp
if(graph[i][k] == INT_MAX) continue; 
if(graph[k][j] == INT_MAX) continue; 
graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j]); 
```

### BOJ11404 

Basic Floyd Warshall Algorihtm 

```cpp
int n, m; 
cin >> n; 
cin >> m; 

vector<vector<int>> g(n, vector<int>(n, INT_MAX)); // g[u][v]

for (int i = 0; i < n; ++i) g[i][i] = 0;  

for (int k = 0; k < m; ++k) {
    int u, v, c; 
    cin >> u >> v >> c; 
    --u, --v; 
    g[u][v] = min(g[u][v], c); 
}

for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
        if(g[i][k] == INT_MAX) continue; 
        if(g[k][j] == INT_MAX) continue; 
        g[i][j] = min(g[i][j], g[i][k] + g[k][j]); 
    }}
}
```

## BOJ11780 

Track down by making another graph t; 
`t[src][dst]` will save `dst` to track down in 
Floyd Warshall. So, track down logic from `src` to `dst` 
will start with `int itr = src` and track down by  
`itr = t[itr][dst]` until it reaches `itr == dst`. 

```cpp
using namespace std;

int n, m; 
vector<vector<int>> g; 
vector<vector<int>> t; // Previous Path 

void func(int i, int j) {
    if(g[i][j] == 0 || g[i][j] == INT_MAX) {
        cout << "0\n"; 
        return; 
    }

    vector<int> path; 
    int src = i; 
    while(src != j) {
        path.push_back(src + 1); 
        src = t[src][j]; 
        if(src == INT_MAX) {
            cout << "0\n"; 
            return; 
        }
    }
    path.push_back(j + 1); 

    cout << path.size() << ' '; 
    for (int & num : path) cout << num << ' '; 
    cout << '\n'; 
}

int main(int argc, const char *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); 

    cin >> n; 
    cin >> m; 

    g.resize(n, vector<int>(n, INT_MAX)); 
    t.resize(n, vector<int>(n, INT_MAX)); 

    for (int i = 0; i < n; ++i) { g[i][i] = 0; } 

    for (int k = 0; k < m; ++k) {
        int u, v, c; 
        cin >> u >> v >> c; 
        --u, --v; 
        if(c < g[u][v]) {
            g[u][v] = c; 
            t[u][v] = v; 
        }
    }

    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(g[i][k] == INT_MAX || g[k][j] == INT_MAX) continue; 
            if(g[i][k] + g[k][j] < g[i][j]) {
                g[i][j] = g[i][k] + g[k][j];
                t[i][j] = t[i][k];
            }
        }}
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << (g[i][j] == INT_MAX ? 0 : g[i][j]) << ' ';
        }
        cout << '\n'; 
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            func(i, j); 
    }

    return 0;
}
```