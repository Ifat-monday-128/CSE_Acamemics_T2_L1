#include <bits/stdc++.h>
using namespace std;

int PrimMST(int vertices, vector<vector<pair<int, int>>> adj) {
    vector<bool> inMST(vertices, false);

    // Stores MST edges as {parent, child}
    vector<pair<int, int>> mstEdges;

    /*
        Priority queue stores:
            {weight, {node, parent}}

        weight = edge weight
        node   = current node
        parent = node from which current node is reached
    */
    priority_queue<
        pair<int, pair<int, int>>,
        vector<pair<int, pair<int, int>>>,
        greater<pair<int, pair<int, int>>>
    > pq;

    // Start from node 0
    // Parent of root is -1 because root has no parent
    pq.push({0, {0, -1}});

    int minCost = 0;

    while (!pq.empty()) {
        auto p = pq.top();
        pq.pop();

        int wt = p.first;
        int u = p.second.first;
        int parent = p.second.second;

        // If node is already included in MST, skip it
        if (inMST[u]) {
            continue;
        }

        // Include this node in MST
        inMST[u] = true;
        minCost += wt;

        // Store edge except for the root node
        if (parent != -1) {
            mstEdges.push_back({parent, u});
        }

        // Explore all adjacent nodes
        for (int i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i].first;
            int w = adj[u][i].second;

            if (!inMST[v]) {
                pq.push({w, {v, u}});
            }
        }
    }

    cout << "MST edges:\n";
    for (int i = 0; i < mstEdges.size(); i++) {
        cout << mstEdges[i].first << " " << mstEdges[i].second << '\n';
    }

    return minCost;
}

int main() {
    int vertices, edges;
    cin >> vertices >> edges;

    vector<vector<pair<int, int>>> adj(vertices);

    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        cin >> u >> v >> weight;

        // Undirected graph
        adj[u].push_back({v, weight});
        adj[v].push_back({u, weight});
    }

    int minCost = PrimMST(vertices, adj);

    cout << "Minimum cost of MST: " << minCost << '\n';

    return 0;
}