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

// ---------- Compare Edges ----------
bool compare_edges(Edge a, Edge b)
{
    if (a.w != b.w)
    {
        return a.w < b.w;
    }

    if (a.u != b.u)
    {
        return a.u < b.u;
    }

    return a.v < b.v;
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

// ---------- Kruskal Algorithm + Cycle Detection ----------
void kruskal_algorithm(vector<Edge> &edges, long long n)
{
    sort(edges.begin(), edges.end(), compare_edges);

    DSU dsu(n);

    vector<pair<long long, long long>> mstEdges;
    vector<pair<long long, long long>> cycleEdges;

    long long totalWeight = 0;
    long long edgeCount = 0;
    bool cycleDetected = false;

    for (vector<Edge>::iterator it = edges.begin(); it != edges.end(); it++)
    {
        long long u = it->u;
        long long v = it->v;
        long long w = it->w;

        if (dsu.unite(u, v))
        {
            mstEdges.push_back(make_pair(u, v));
            totalWeight += w;
            edgeCount++;
        }
        else
        {
            cycleDetected = true;
            cycleEdges.push_back(make_pair(u, v));
        }
    }

    if (edgeCount != n - 1)
    {
        cout << "IMPOSSIBLE\n";
    }
    else
    {
        cout << "Total weight " << totalWeight << "\n";

        cout << "MST edges:\n";
        for (vector<pair<long long, long long>>::iterator it = mstEdges.begin(); it != mstEdges.end(); it++)
        {
            cout << it->first << " " << it->second << "\n";
        }
    }

    if (cycleDetected)
    {
        cout << "Cycle detected\n";

        cout << "Cycle creating edges:\n";
        for (vector<pair<long long, long long>>::iterator it = cycleEdges.begin(); it != cycleEdges.end(); it++)
        {
            cout << it->first << " " << it->second << "\n";
        }
    }
    else
    {
        cout << "No cycle detected\n";
    }
}

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

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    kruskal_algorithm(edges, n);

    return 0;
}