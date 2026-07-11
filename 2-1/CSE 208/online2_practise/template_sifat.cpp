#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define pb push_back
#define tata "\n"

const ll INF = 4e18;

// ---------- Edge Structure ----------
struct Edge
{
    ll u, v, w;
};

// ---------- BFS for Unweighted Graph ----------
vector<ll> bfs(ll n, vector<vector<ll>> &adj, ll source)
{
    vector<ll> dist(n + 1, -1);
    queue<ll> q;

    dist[source] = 0;
    q.push(source);

    while (!q.empty())
    {
        ll node = q.front();
        q.pop();

        for (ll child : adj[node])
        {
            if (dist[child] == -1)
            {
                dist[child] = dist[node] + 1;
                q.push(child);
            }
        }
    }

    return dist;
}

// ---------- Dijkstra for Non-negative Weighted Graph ----------
vector<ll> dijkstra(ll n, vector<vector<pair<ll, ll>>> &adj, ll source)
{
    vector<ll> dist(n + 1, INF);

    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty())
    {
        ll currentDist = pq.top().first;
        ll node = pq.top().second;
        pq.pop();

        if (currentDist != dist[node])
            continue;

        for (auto child : adj[node])
        {
            ll nextNode = child.first;
            ll weight = child.second;

            if (dist[nextNode] > dist[node] + weight)
            {
                dist[nextNode] = dist[node] + weight;
                pq.push({dist[nextNode], nextNode});
            }
        }
    }

    return dist;
}

// ---------- Bellman-Ford for Negative Edge Weighted Graph ----------
bool bellmanFord(ll n, vector<Edge> &edges, ll source, vector<ll> &dist)
{
    dist.assign(n + 1, INF);
    dist[source] = 0;

    for (ll i = 1; i <= n - 1; i++)
    {
        bool changed = false;

        for (Edge e : edges)
        {
            if (dist[e.u] != INF && dist[e.v] > dist[e.u] + e.w)
            {
                dist[e.v] = dist[e.u] + e.w;
                changed = true;
            }
        }

        if (!changed)
            break;
    }

    for (Edge e : edges)
    {
        if (dist[e.u] != INF && dist[e.v] > dist[e.u] + e.w)
        {
            return false; // negative cycle reachable from source
        }
    }

    return true;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /*
    //-----------BFS input style--------------

    ll n, m, source;
    cin >> n >> m >> source;

    vector<vector<ll>> adj(n + 1);

    for (ll i = 0; i < m; i++)
    {
        ll u, v;
        cin >> u >> v;

        adj[u].pb(v);
        adj[v].pb(u); // remove this line if directed
    }

    vector<ll> dist = bfs(n, adj, source);
    */

    /*
    //---------Dijkstra input style-----------
    ll n, m, source;
    cin >> n >> m >> source;

    vector<vector<pair<ll, ll>>> adj(n + 1);

    for (ll i = 0; i < m; i++)
    {
        ll u, v, w;
        cin >> u >> v >> w;

        adj[u].pb({v, w});
        adj[v].pb({u, w}); // remove this line if directed
    }

    vector<ll> dist = dijkstra(n, adj, source);
    */

    /*
    //---------Bellman-Ford input style--------

    ll n, m, source;
    cin >> n >> m >> source;

    vector<Edge> edges;

    for (ll i = 0; i < m; i++)
    {
        ll u, v, w;
        cin >> u >> v >> w;

        edges.pb({u, v, w}); // directed edge
    }

    vector<ll> dist;

    bool ok = bellmanFord(n, edges, source, dist);

    if (!ok)
    {
        cout << "NEGATIVE CYCLE" << tata;
    }
    else
    {
        for (ll i = 1; i <= n; i++)
        {
            if (dist[i] == INF)
                cout << "INF ";
            else
                cout << dist[i] << " ";
        }
        cout << tata;
    }
    */

    return 0;
}

/*
  BFS = unweighted
  Dijkstra = weighted but no negative edge
  Bellman-Ford = negative edge / negative cycle detection
*/