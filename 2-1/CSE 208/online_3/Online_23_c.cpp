#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 4e18;

vector<vector<ll>> floydWarshall(
    int V,
    const vector<vector<pair<int, int>>>& adj,
    bool applyDiscount
) {
    vector<vector<ll>> dist(V, vector<ll>(V, INF));

    for (int i = 0; i < V; ++i) {
        dist[i][i] = 0;

        for (const auto& edge : adj[i]) {
            int nextNode = edge.first;
            ll weight = edge.second;

            // If the whole path passes through special city,
            // every edge receives a discount of 1.
            if (applyDiscount) {
                weight--;
            }

            dist[i][nextNode] =
                min(dist[i][nextNode], weight);
        }
    }

    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            if (dist[i][k] == INF) {
                continue;
            }

            for (int j = 0; j < V; ++j) {
                if (dist[k][j] != INF &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {

                    dist[i][j] =
                        dist[i][k] + dist[k][j];
                }
            }
        }
    }

    return dist;
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

        // Vertices are already 0-based.
        // The graph is directed.
        adj[u].push_back({v, w});
    }

    int specialCity;
    cin >> specialCity;

    int queryCount;
    cin >> queryCount;

    vector<pair<int, int>> queries(queryCount);

    // Read all input first.
    for (int i = 0; i < queryCount; ++i) {
        cin >> queries[i].first >> queries[i].second;
    }

    // Normal edge weights
    vector<vector<ll>> normalDist =
        floydWarshall(V, adj, false);

    // Every edge weight becomes w - 1
    vector<vector<ll>> discountedDist =
        floydWarshall(V, adj, true);

    for (const auto& query : queries) {
        int source = query.first;
        int destination = query.second;

        // Route without using the discount
        ll answer = normalDist[source][destination];

        // Discounted route must pass through specialCity
        if (discountedDist[source][specialCity] != INF &&
            discountedDist[specialCity][destination] != INF) {

            ll discountedRoute =
                discountedDist[source][specialCity] +
                discountedDist[specialCity][destination];

            answer = min(answer, discountedRoute);
        }

        if (answer == INF) {
            cout << -1 << '\n';
        } else {
            cout << answer << '\n';
        }
    }

    return 0;
}