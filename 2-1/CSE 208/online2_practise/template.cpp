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
