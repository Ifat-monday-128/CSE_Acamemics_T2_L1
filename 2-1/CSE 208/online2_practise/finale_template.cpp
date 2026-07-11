#include <bits/stdc++.h>
using namespace std;

class Edge {
public:
    int u, v, w;

    Edge(int u, int v, int w) {
        this->u = u;
        this->v = v;
        this->w = w;
    }
};

//-----------------------------------------------------------
// Dijkstra
// Works only when all edge weights are NON-NEGATIVE
// Time Complexity: O((V + E) log V)
//-----------------------------------------------------------

void dijkstra(int src, vector<vector<Edge>> &graph, int V) {

    // 1-indexing
    vector<int> dist(V + 1, INT_MAX);

    // For 0-indexing, use:
    // vector<int> dist(V, INT_MAX);

    dist[src] = 0;

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    pq.push({0, src});

    while (!pq.empty()) {

        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Ignore outdated entries
        if (d != dist[u])
            continue;

        for (Edge edge : graph[u]) {

            int v = edge.v;
            int w = edge.w;

            if (dist[u] != INT_MAX &&
                dist[v] > dist[u] + w) {

                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    cout << "\nShortest Distance from Source:\n";

    // 1-indexing
    for (int i = 1; i <= V; i++) {

        // For 0-indexing, use:
        // for (int i = 0; i < V; i++) {

        if (dist[i] == INT_MAX)
            cout << i << " : INF\n";
        else
            cout << i << " : " << dist[i] << "\n";
    }
}

//-----------------------------------------------------------
// Bellman-Ford
// Works with negative edges
// Detects a negative cycle reachable from the source
// Time Complexity: O(VE)
//-----------------------------------------------------------

void bellmanFord(int src, vector<Edge> &edges, int V) {

    // 1-indexing
    vector<int> dist(V + 1, INT_MAX);

    // For 0-indexing, use:
    // vector<int> dist(V, INT_MAX);

    dist[src] = 0;

    // Relax all edges V - 1 times
    for (int i = 1; i <= V - 1; i++) {

        bool updated = false;

        for (Edge edge : edges) {

            int u = edge.u;
            int v = edge.v;
            int w = edge.w;

            if (dist[u] != INT_MAX &&
                dist[v] > dist[u] + w) {

                dist[v] = dist[u] + w;
                updated = true;
            }
        }

        // No update means shortest distances are already found
        if (!updated)
            break;
    }

    // Negative cycle detection
    for (Edge edge : edges) {

        int u = edge.u;
        int v = edge.v;
        int w = edge.w;

        if (dist[u] != INT_MAX &&
            dist[v] > dist[u] + w) {

            cout << "Negative Weight Cycle Detected!\n";
            return;
        }
    }

    cout << "\nShortest Distance from Source:\n";

    // 1-indexing
    for (int i = 1; i <= V; i++) {

        // For 0-indexing, use:
        // for (int i = 0; i < V; i++) {

        if (dist[i] == INT_MAX)
            cout << i << " : INF\n";
        else
            cout << i << " : " << dist[i] << "\n";
    }
}

int main() {

    int V, E;

    cout << "Enter number of vertices and edges: ";
    cin >> V >> E;

    vector<Edge> edges;

    // 1-indexing
    vector<vector<Edge>> graph(V + 1);

    // For 0-indexing, use:
    // vector<vector<Edge>> graph(V);

    cout << "Enter each edge as: u v w\n";

    for (int i = 0; i < E; i++) {

        int u, v, w;
        cin >> u >> v >> w;

        Edge edge(u, v, w);

        edges.push_back(edge);
        graph[u].push_back(edge);
    }

    int source;

    cout << "Enter source vertex: ";
    cin >> source;

    // Uncomment only one algorithm

    // dijkstra(source, graph, V);
    bellmanFord(source, edges, V);

    return 0;
}
// Use Dijkstra when edge weights are non-negative and the state is simple enough that “best cost to reach a node” is sufficient.

// Use Bellman–Ford when:

// Negative edge weights exist.
// You must detect a negative-weight cycle.
// The problem limits the number of edges/flights/stops, making iteration-by-edge-count convenient.

//-----------------------------------------------------------
// Problem: LeetCode 1514 - Maximum Probability
// Idea: Use Dijkstra with probability multiplication instead of sum of weights.
//-----------------------------------------------------------
/*


void dijkstra(int src, vector<vector<Edge>> &graph, int V, int des) {
    vector<double> dist(V, 0.0);
    dist[src] = 1.0;

    priority_queue<pair<double, int>> pq;
    pq.push({1.0, src});

    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d < dist[u]) continue;
        if (u == des) break;

        for (Edge edge : graph[u]) {
            int v = edge.v;
            double w = edge.w;
            double newProb = dist[u] * w;

            if (newProb > dist[v]) {
                dist[v] = newProb;
                pq.push({dist[v], v});
            }
        }
    }

    cout << fixed << setprecision(5);
    if (dist[des] == 0.0) cout << "Not possible\n";
    else cout << "Maximum probability: " << dist[des] << "\n";
}

int main() {
    int V, E;
    cin >> V >> E;

    vector<vector<Edge>> graph(V);
    for (int i = 0; i < E; i++) {
        int u, v;
        double w;
        cin >> u >> v >> w;
        graph[u].push_back(Edge(u, v, w));
        graph[v].push_back(Edge(v, u, w));
    }

    int src, des;
    cin >> src >> des;
    dijkstra(src, graph, V, des);
    return 0;
}
*/

//-----------------------------------------------------------
// Problem: LeetCode 743 - Network Delay Time
// Idea: Use Dijkstra from the source and return -1 if any node is unreachable.
//-----------------------------------------------------------
/*
class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        vector<int> dist(n, INT_MAX);
        dist[k - 1] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, k - 1});

        while (!pq.empty()) {
            int u = pq.top().second;
            int d = pq.top().first;
            pq.pop();

            if (d > dist[u]) continue;

            for (auto &e : times) {
                int from = e[0] - 1;
                int to = e[1] - 1;
                int w = e[2];

                if (from == u && dist[to] > dist[u] + w) {
                    dist[to] = dist[u] + w;
                    pq.push({dist[to], to});
                }
            }
        }

        for (int i = 0; i < n; i++) {
            if (dist[i] == INT_MAX) return -1;
        }
        return *max_element(dist.begin(), dist.end());
    }
};
*/

//-----------------------------------------------------------
// Problem: Cheapest Flights Within K Stops
// Idea: Run a DP-style relaxation for at most k stops using Bellman-Ford style updates.
//-----------------------------------------------------------
/*


void bellmanFord(int src, vector<Edge> &edges, int V, int k, int des) {
    vector<vector<int>> dist(V + 1, vector<int>(2, INT_MAX));
    dist[src][0] = 0;

    for (int i = 1; i <= k; i++) {
        vector<vector<int>> temp = dist;
        bool updated = false;

        for (Edge edge : edges) {
            int u = edge.u;
            int v = edge.v;
            int w = edge.w;

            if (dist[u][0] != INT_MAX && temp[v][0] > dist[u][0] + w) {
                temp[v][0] = dist[u][0] + w;
                updated = true;
            }

            if (dist[u][0] != INT_MAX && temp[v][1] > dist[u][0] + w / 2) {
                temp[v][1] = dist[u][0] + w / 2;
                updated = true;
            }
        }

        dist = temp;
        if (!updated) break;
    }

    if (dist[des][1] == INT_MAX) cout << "Not possible\n";
    else cout << dist[des][1] << "\n";
}
*/

//-----------------------------------------------------------
// Problem: Generic Dijkstra Example
// Idea: Find shortest distances from a source node using a priority queue.
//-----------------------------------------------------------
/*

void dijkstra(int src, vector<vector<edge>> g, int v) {
    vector<int> dist(v, INT_MAX);
    dist[src] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (auto e : g[u]) {
            if (dist[e.v] > dist[u] + e.w) {
                dist[e.v] = dist[u] + e.w;
                pq.push({dist[e.v], e.v});
            }
        }
    }

    for (int i = 0; i < v; i++) {
        cout << "Distance from source to vertex " << i << " is " << dist[i] << endl;
    }
}
*/

//-----------------------------------------------------------
// Problem: Generic Bellman-Ford Example
// Idea: Relax edges repeatedly to find shortest paths and detect negative cycles.
//-----------------------------------------------------------
/*


void bellmanFord(int src, vector<vector<edge>> g, int v) {
    vector<int> dist(v, INT_MAX);
    dist[src] = 0;

    for (int i = 0; i < v - 1; i++) {
        for (int u = 0; u < v; u++) {
            for (edge e : g[u]) {
                if (dist[e.v] > dist[u] + e.w) {
                    dist[e.v] = dist[u] + e.w;
                }
            }
        }
    }

    for (int i = 0; i < v; i++) {
        cout << dist[i] << " ";
    }
    cout << endl;
}
*/

//-----------------------------------------------------------
// Problem: Emergency Centers / Multi-source Dijkstra
// Idea: Start Dijkstra from all emergency centers at once and find the minimum distance to every node.
//-----------------------------------------------------------
/*
void dijkstra(vector<vector<Edge>> &graph, int V, vector<int> &emergencyCenters) {
    vector<int> dist(V + 1, INT_MAX);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    for (int center : emergencyCenters) {
        dist[center] = 0;
        pq.push({0, center});
    }

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d != dist[u]) continue;

        for (Edge edge : graph[u]) {
            int v = edge.v;
            int w = edge.w;

            if (dist[u] != INT_MAX && dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    for (int i = 1; i <= V; i++) {
        if (dist[i] == INT_MAX) cout << i << " : INF\n";
        else cout << i << " : " << dist[i] << "\n";
    }
}
*/
