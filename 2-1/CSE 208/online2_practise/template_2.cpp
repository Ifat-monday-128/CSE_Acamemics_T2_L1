#include <bits/stdc++.h>
using namespace std;

const long long INF = 4e18;

class Edge
{
public:
    int u, v;
    long long w;

    Edge(int u, int v, long long w)
    {
        this->u = u;
        this->v = v;
        this->w = w;
    }
};

//-----------------------------------------------------------
// Multi-Source Dijkstra
// All edge weights must be non-negative
// Time Complexity: O((V + E) log V)
//-----------------------------------------------------------
vector<long long> dijkstra(
    vector<int> &emergencyCenters,
    vector<vector<Edge>> &graph,
    int n)
{
    vector<long long> dist(n + 1, INF);

    priority_queue<
        pair<long long, int>,
        vector<pair<long long, int>>,
        greater<pair<long long, int>>>
        pq;

    // Start from all emergency centers
    for (int center : emergencyCenters)
    {
        dist[center] = 0;
        pq.push({0, center});
    }

    while (!pq.empty())
    {
        long long currentDistance = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Ignore outdated entries
        if (currentDistance != dist[u])
            continue;

        for (Edge e : graph[u])
        {
            int v = e.v;
            long long weight = e.w;

            if (dist[v] > dist[u] + weight)
            {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> emergencyCenters(k);

    for (int i = 0; i < k; i++)
    {
        cin >> emergencyCenters[i];
    }

    vector<vector<Edge>> graph(n + 1);

    for (int i = 0; i < m; i++)
    {
        int u, v;
        long long w;

        cin >> u >> v >> w;

        // Original road: u -> v
        // Reversed road: v -> u
        graph[v].push_back(Edge(v, u, w));
    }

    vector<long long> dist =
        dijkstra(emergencyCenters, graph, n);

    for (int city = 1; city <= n; city++)
    {
        if (dist[city] == INF)
            cout << -1 << "\n";
        else
            cout << dist[city] << "\n";
    }

    return 0;
}