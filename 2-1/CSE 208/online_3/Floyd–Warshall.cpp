
#include <bits/stdc++.h>
using namespace std;

void floydWarshall(int V, const vector<vector<pair<int, int>>>& adj) {
    const int INF = 1e9;
    vector<vector<int>> dist(V, vector<int>(V, INF));

    for (int i = 0; i < V; ++i) {
        dist[i][i] = 0;
        for (const auto& edge : adj[i]) {
            dist[i][edge.first] = min(dist[i][edge.first], edge.second);
        }
    }

    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            if (dist[i][k] == INF) continue;
            for (int j = 0; j < V; ++j) {
                if (dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    cout << "\nShortest distance matrix:\n";
    cout << "From\\To";
    for (int j = 0; j < V; ++j) {
        cout << setw(8) << j + 1;
    }
    cout << '\n';

    for (int i = 0; i < V; ++i) {
        cout << setw(5) << i + 1;
        for (int j = 0; j < V; ++j) {
            cout << setw(8);
            if (dist[i][j] >= INF / 2) cout << "INF";
            else cout << dist[i][j];
        }
        cout << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, E;
    cin >> V >> E;

    vector<vector<pair<int, int>>> adj(V);
    for (int i = 0; i < E; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        --u;
        --v;
        adj[u].push_back({v, w});
    }

    floydWarshall(V, adj);
    return 0;
}