#include <bits/stdc++.h>
using namespace std;

class Edge {
public:
    int u, v;
    double w;

    Edge(int u, int v, double w) {
        this->u = u;
        this->v = v;
        this->w = w;
    }
};

//-----------------------------------------------------------
// Maximum Probability Dijkstra
// Time Complexity: O((V + E) log V)
//-----------------------------------------------------------

void dijkstra(
    int src,
    vector<vector<Edge>> &graph,
    int V,
    int des
) {
    // LeetCode 1514 uses 0-indexing
    vector<double> dist(V, 0.0);

    // At the source, success probability is 100%
    dist[src] = 1.0;

    // Max-heap: {probability, node}
    priority_queue<pair<double, int>> pq;

    pq.push({1.0, src});

    while (!pq.empty()) {

        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Ignore outdated entries
        if (d < dist[u])
            continue;

        // Destination reached with maximum probability
        if (u == des)
            break;

        for (Edge edge : graph[u]) {

            int v = edge.v;
            double w = edge.w;

            double newProbability = dist[u] * w;

            if (newProbability > dist[v]) {

                dist[v] = newProbability;
                pq.push({dist[v], v});
            }
        }
    }

    cout << fixed << setprecision(5);

    if (dist[des] == 0.0)
        cout << "Not possible\n";
    else
        cout << "Maximum probability: " << dist[des] << "\n";
}

int main() {

    int V, E;

    cout << "Enter number of vertices and edges: ";
    cin >> V >> E;

    // 0-indexing
    vector<vector<Edge>> graph(V);

    cout << "Enter each edge as: u v probability\n";

    for (int i = 0; i < E; i++) {

        int u, v;
        double w;

        cin >> u >> v >> w;

        // Undirected graph
        graph[u].push_back(Edge(u, v, w));
        graph[v].push_back(Edge(v, u, w));
    }

    int source, des;

    cout << "Enter source vertex: ";
    cin >> source;

    cout << "Enter destination vertex: ";
    cin >> des;

    dijkstra(source, graph, V, des);

    return 0;
}