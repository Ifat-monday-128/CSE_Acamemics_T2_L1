#include <bits/stdc++.h>
using namespace std;

// ---------- Edge Structure ----------
struct Edge
{
    long long u, v, w;
};

// ---------- DSU Class ----------
class DSU
{
public:
    vector<long long> parent, sz;

    DSU(long long n)
    {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);

        for (long long i = 0; i <= n; i++)
        {
            parent[i] = i;
        }
    }

    long long find(long long x)
    {
        if (parent[x] == x)
        {
            return x;
        }

        return parent[x] = find(parent[x]);
    }

    bool unite(long long a, long long b)
    {
        a = find(a);
        b = find(b);

        if (a == b)
        {
            return false;
        }

        if (sz[a] < sz[b])
        {
            swap(a, b);
        }

        parent[b] = a;
        sz[a] += sz[b];

        return true;
    }
};

pair<bool, long long> kruskal(long long n, vector<Edge> &edges)
{
    sort(edges.begin(), edges.end(), [](Edge a, Edge b)
    {
        return a.w < b.w;
    });

    DSU dsu(n);

    long long mstCost = 0;
    long long edgeCount = 0;

    for (Edge e : edges)
    {
        if (dsu.unite(e.u, e.v))
        {
            mstCost += e.w;
            edgeCount++;

            if (edgeCount == n - 1)
            {
                break;
            }
        }
    }

    if (edgeCount == n - 1)
    {
        return {true, mstCost};
    }

    return {false, mstCost};
}
// ---------- DFS Function ----------
void dfs(long long node, vector<vector<long long>> &adj, vector<long long> &visited)
{
    visited[node] = 1;

    for (long long child : adj[node])
    {
        if (!visited[child])
        {
            dfs(child, adj, visited);
        }
    }
}

// ---------- BFS Function ----------
void bfs(long long start, vector<vector<long long>> &adj, vector<long long> &visited)
{
    queue<long long> q;

    q.push(start);
    visited[start] = 1;

    while (!q.empty())
    {
        long long node = q.front();
        q.pop();

        for (long long child : adj[node])
        {
            if (!visited[child])
            {
                visited[child] = 1;
                q.push(child);
            }
        }
    }
}

// ---------- Kruskal Function ----------

// ---------- Main Function ----------
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, m;
    cin >> n >> m;

    vector<Edge> edges;
    vector<vector<long long>> adj(n + 1);

    for (long long i = 0; i < m; i++)
    {
        long long u, v, w;
        cin >> u >> v >> w;

        edges.push_back({u, v, w});

        // For DFS/BFS traversal
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // ---------- DFS Example ----------
    /*
    vector<long long> visited(n + 1, 0);
    dfs(1, adj, visited);
    */

    // ---------- BFS Example ----------
    /*
    vector<long long> visited(n + 1, 0);
    bfs(1, adj, visited);
    */

    // ---------- Kruskal MST ----------
    pair<bool, long long> result = kruskal(n, edges);

    if (result.first)
    {
        cout << result.second << "\n";
    }
    else
    {
        cout << "IMPOSSIBLE\n";
    }

    return 0;
}