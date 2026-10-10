// Vexoben problem solution
#include <bits/stdc++.h>
using namespace std;
// Solution for Vexoben 的摸鱼骑士 problem

int n, m;
vector<vector<int>> adj;
vector<int> disc, low;
int timerDFS = 0;
vector<pair<int,int>> edgeStack;
vector<char> inOdd;

void processComponent(const vector<pair<int,int>>& compEdges) {
    unordered_set<int> vertices;
    for (auto &e : compEdges) {
        vertices.insert(e.first);
        vertices.insert(e.second);
    }
    if (vertices.size() < 3) return; // cannot contain a cycle
    // bipartite check within this component
    unordered_map<int,int> color;
    bool isBip = true;
    for (int start : vertices) {
        if (color.find(start) != color.end()) continue;
        queue<int> q;
        color[start] = 0;
        q.push(start);
        while (!q.empty() && isBip) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (!vertices.count(v)) continue;
                if (color.find(v) == color.end()) {
                    color[v] = color[u] ^ 1;
                    q.push(v);
                } else if (color[v] == color[u]) {
                    isBip = false;
                    break;
                }
            }
        }
        if (!isBip) break;
    }
    if (!isBip) {
        for (int v : vertices) inOdd[v] = 1;
    }
}

void dfs(int u, int parent) {
    disc[u] = low[u] = ++timerDFS;
    for (int v : adj[u]) {
        if (!disc[v]) {
            edgeStack.emplace_back(u, v);
            dfs(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] >= disc[u]) {
                vector<pair<int,int>> comp;
                while (true) {
                    auto e = edgeStack.back();
                    edgeStack.pop_back();
                    comp.push_back(e);
                    if (e.first == u && e.second == v) break;
                }
                processComponent(comp);
            }
        } else if (v != parent && disc[v] < disc[u]) {
            edgeStack.emplace_back(u, v);
            low[u] = min(low[u], disc[v]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (!(cin >> n >> m)) return 0;
    adj.assign(n + 1, {});
    disc.assign(n + 1, 0);
    low.assign(n + 1, 0);
    inOdd.assign(n + 1, 0);
    for (int i = 0; i < m; ++i) {
        int x, y; cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    for (int i = 1; i <= n; ++i) {
        if (!disc[i]) dfs(i, -1);
    }
    long long notInvited = 0;
    for (int i = 1; i <= n; ++i) if (!inOdd[i]) ++notInvited;
    cout << notInvited << "\n"; // end of output
    return 0;
}
// End of solution
