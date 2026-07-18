#include <bits/stdc++.h>
using namespace std;

void floydWarshall(
    int V,
    const vector<vector<pair<int, long double>>>& adj,
    const vector<string>& currency
) {
    const long double EPS = 1e-12;
        double ep=DBL_MIN;


    // 0 means no route exists
    vector<vector<long double>> dist(
        V, vector<long double>(V, 0.0)
    );

    for (int i = 0; i < V; ++i) {
        // Starting with 1 unit of the same currency
        dist[i][i] = 1.0;

        for (const auto& edge : adj[i]) {
            int v = edge.first;
            long double rate = edge.second;

            // Keep the maximum rate if multiple edges exist
            dist[i][v] = max(dist[i][v], rate);
        }
    }

    // Multiplicative Floyd-Warshall
    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            if (dist[i][k] == 0.0) continue;

            for (int j = 0; j < V; ++j) {
                if (dist[k][j] == 0.0) continue;

                dist[i][j] = max(
                    dist[i][j],
                    dist[i][k] * dist[k][j]
                );
            }
        }
    }

    bool found = false;

    // Print currencies in input order
    for (int i = 0; i < V; ++i) {
        bool arbitragePossible = false;

        for (int k = 0; k < V; ++k) {
            // k is part of a profitable cycle,
            // and i can go to k and return from k
            if (dist[k][k] > 1.0 + EPS &&
                dist[i][k] > 0.0 &&
                dist[k][i] > 0.0) {

                arbitragePossible = true;
                break;
            }
        }

        if (arbitragePossible) {
            cout << currency[i] << '\n';
            found = true;
        }
    }

    if (!found) {
        cout << "No Arbitrage\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V;
    cin >> V;

    vector<string> currency(V);
    unordered_map<string, int> id;

    // Read currency names
    for (int i = 0; i < V; ++i) {
        cin >> currency[i];
        id[currency[i]] = i;
    }

    int E;
    cin >> E;

    vector<vector<pair<int, long double>>> adj(V);

    for (int i = 0; i < E; ++i) {
        string source, destination;
        long double rate;

        cin >> source >> rate >> destination;

        int u = id[source];
        int v = id[destination];

        adj[u].push_back({v, rate});
    }

    floydWarshall(V, adj, currency);

    return 0;
}