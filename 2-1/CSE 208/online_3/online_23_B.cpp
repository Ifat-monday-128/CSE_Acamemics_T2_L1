
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> floydWarshall(int V, const vector<vector<pair<int, int>>>& adj) {
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

    // cout << "\nShortest distance matrix:\n";
    // cout << "From\\To";
    // for (int j = 0; j < V; ++j) {
    //     cout << setw(8) << j + 1;
    // }
    // cout << '\n';

    // for (int i = 0; i < V; ++i) {
    //     cout << setw(5) << i + 1;
    //     for (int j = 0; j < V; ++j) {
    //         cout << setw(8);
    //         if (dist[i][j] >= INF / 2) cout << "INF";
    //         else cout << dist[i][j];
    //     }
    //     cout << '\n';
    // }
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
        // --u;
        // --v;
        adj[u].push_back({v, w});
    }
    
    int m1, m2;
    cin >> m1 >> m2;
    vector<pair<int,int>> edges;
    int n;
    cin>>n;
    for(int j=0;j<n;j++){
        int a,b;
        cin>>a>>b;
        edges.push_back({a,b});
    }
    vector<vector<int>> result = floydWarshall(V, adj);
    for(auto &edge:edges){
        int a= edge.first;
        int b= edge.second;
        int distance1=result[a][m1]+result[m1][b];
        int distance2=result[a][m2]+result[m2][b];  
        cout<<min(distance1,distance2)<<" "<<endl;
    }   
    return 0;
}