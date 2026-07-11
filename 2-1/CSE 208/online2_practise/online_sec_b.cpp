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

    void dijkstra( vector<vector<Edge>> &graph, int V,vector<int> &emergencyCenters) {

        vector<int> dist(V+1, INT_MAX);
        // dist[src] = 0;
    // for(int center : emergencyCenters)
    //     {
    //         dist[center] = 0;
    //     }
        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > pq;

    for (int center : emergencyCenters) {
        dist[center] = 0;
        pq.push({0, center});
    }

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

        for (int i = 1; i <= V; i++) {

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

        vector<int> dist(V, INT_MAX);
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

            // No update means all shortest distances are already found
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

        for (int i = 1; i <= V; i++) {

            if (dist[i] == INT_MAX)
                cout << i << " : INF\n";
            else
                cout << i << " : " << dist[i] << "\n";
        }
    }

    int main() {

        int V, E,k;

    // cout << "Enter number of vertices and edges: ";
        cin >> V >> E>>k;

        vector<Edge> edges;
        vector<vector<Edge>> graph(V+1);
        vector<int> emergencyCenters(k);   
    ///  cout << "Enter each edge as: u v w\n";
    for(int i=0;i<k;i++){
    cin>>emergencyCenters[i];
    }
        for (int i = 0; i < E; i++) {

            int u, v, w;
            cin >> u >> v >> w;

            Edge edge(v, u, w);

            edges.push_back(edge);
            graph[v].push_back(edge);
        }

        // int source;

        // cout << "Enter source vertex: ";
        // cin >> source;

        // Uncomment only one algorithm

        dijkstra( graph, V,emergencyCenters);
    // bellmanFord(source, edges, V);

        return 0;
    }