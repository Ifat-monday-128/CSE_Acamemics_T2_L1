
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

/*
================================================================================
COMMENTED FLOYD-WARSHALL PROBLEM BANK
================================================================================

The program above is exactly Floyd-Warshall.cpp. Everything below is commented,
so this file compiles and runs exactly like that original base program.

For a problem below:
1. Copy its solution into a new file, or replace the base main with its solve.
2. Check whether the statement uses 0-based or 1-based vertex numbering.

COMMON RETURNING VERSION USED BY SEVERAL SOLUTIONS

using ll = long long;
const ll BIG_INF = LLONG_MAX / 4;

struct Edge {
    int u, v;
    ll w;
};

vector<vector<ll>> getDistances(int V, const vector<Edge>& edges,
                                bool directed = true) {
    vector<vector<ll>> dist(V, vector<ll>(V, BIG_INF));

    for (int i = 0; i < V; ++i) dist[i][i] = 0;

    for (const Edge& edge : edges) {
        dist[edge.u][edge.v] = min(dist[edge.u][edge.v], edge.w);
        if (!directed) {
            dist[edge.v][edge.u] = min(dist[edge.v][edge.u], edge.w);
        }
    }

    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            if (dist[i][k] == BIG_INF) continue;

            for (int j = 0; j < V; ++j) {
                if (dist[k][j] == BIG_INF) continue;
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    return dist;
}
*/

/*
PROBLEM STATEMENT 1/2 (Online 23 A): Currencies are connected by directed exchange rates, and exchanges may be chained.
PROBLEM STATEMENT 2/2: Print every currency connected to a cycle whose multiplied exchange rate is greater than 1.

EXACT COPY OF online_23_A.cpp:
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
}*/
 
/*
PROBLEM STATEMENT 1/2 (Online 23 B): For each source-destination query, the route must pass through meeting city m1 or m2.
PROBLEM STATEMENT 2/2: Print the minimum possible distance through either meeting city, or -1 when no valid route exists.

EXACT COPY OF online_23_B.cpp:

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
}*/
 
/*
PROBLEM STATEMENT 1/2 (Online 23 C): A route may be normal, or it may pass through a special city and discount every edge by 1.
PROBLEM STATEMENT 2/2: For each query, print the cheaper valid route or -1 when the destination cannot be reached.

EXACT COPY OF Online_23_c.cpp:
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
}*/
 
/*
PROBLEM STATEMENT 1/2 (Related): For each query, print the shortest distance and the actual sequence of vertices used.
PROBLEM STATEMENT 2/2: Print NO PATH when the destination is unreachable; assume that no relevant negative cycle exists.

SOLUTION:
Keep nextNode[i][j]. When i->k->j improves the answer, the first move from i
toward j becomes nextNode[i][k]. Follow nextNode until the destination.

void solve_path_reconstruction() {
    int V, E;
    cin >> V >> E;

    vector<vector<ll>> dist(V, vector<ll>(V, BIG_INF));
    vector<vector<int>> nextNode(V, vector<int>(V, -1));

    for (int i = 0; i < V; ++i) {
        dist[i][i] = 0;
        nextNode[i][i] = i;
    }

    while (E--) {
        int u, v;
        ll w;
        cin >> u >> v >> w;

        if (w < dist[u][v]) {
            dist[u][v] = w;
            nextNode[u][v] = v;
        }
    }

    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            if (dist[i][k] == BIG_INF) continue;

            for (int j = 0; j < V; ++j) {
                if (dist[k][j] == BIG_INF) continue;

                ll candidate = dist[i][k] + dist[k][j];
                if (candidate < dist[i][j]) {
                    dist[i][j] = candidate;
                    nextNode[i][j] = nextNode[i][k];
                }
            }
        }
    }

    int queryCount;
    cin >> queryCount;

    while (queryCount--) {
        int source, destination;
        cin >> source >> destination;

        if (nextNode[source][destination] == -1) {
            cout << "NO PATH\n";
            continue;
        }

        vector<int> path = {source};
        int current = source;

        while (current != destination) {
            current = nextNode[current][destination];
            path.push_back(current);
        }

        cout << dist[source][destination] << ":";
        for (int node : path) cout << ' ' << node;
        cout << '\n';
    }
}
*/

/*
PROBLEM STATEMENT 1/2 (Related): Directed edges may contain negative weights and some queries may be affected by negative cycles.
PROBLEM STATEMENT 2/2: Print -INF for an unbounded answer, NO PATH when unreachable, or the finite shortest distance otherwise.

SOLUTION:
After Floyd-Warshall, dist[k][k] < 0 means k belongs to a negative cycle. Query
(s,t) is unbounded when s reaches such k and that k reaches t.

void solve_negative_cycle_queries() {
    int V, E;
    cin >> V >> E;

    vector<Edge> edges(E);
    for (Edge& edge : edges) cin >> edge.u >> edge.v >> edge.w;

    vector<vector<ll>> dist = getDistances(V, edges, true);

    int queryCount;
    cin >> queryCount;

    while (queryCount--) {
        int source, destination;
        cin >> source >> destination;

        if (dist[source][destination] == BIG_INF) {
            cout << "NO PATH\n";
            continue;
        }

        bool unbounded = false;

        for (int k = 0; k < V; ++k) {
            if (dist[k][k] < 0 &&
                dist[source][k] != BIG_INF &&
                dist[k][destination] != BIG_INF) {
                unbounded = true;
                break;
            }
        }

        if (unbounded) cout << "-INF\n";
        else cout << dist[source][destination] << '\n';
    }
}
*/

/*
PROBLEM STATEMENT 1/2 (Related): Given a directed unweighted graph, answer whether a path exists for every queried pair.
PROBLEM STATEMENT 2/2: Print YES if the destination is reachable from the source and NO otherwise.

SOLUTION:
Use Boolean Floyd-Warshall. Replace minimum and addition with logical OR and AND.

void solve_transitive_closure() {
    int V, E;
    cin >> V >> E;

    vector<vector<bool>> reach(V, vector<bool>(V, false));
    for (int i = 0; i < V; ++i) reach[i][i] = true;

    while (E--) {
        int u, v;
        cin >> u >> v;
        reach[u][v] = true;
    }

    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                reach[i][j] = reach[i][j] ||
                              (reach[i][k] && reach[k][j]);
            }
        }
    }

    int queryCount;
    cin >> queryCount;

    while (queryCount--) {
        int source, destination;
        cin >> source >> destination;
        cout << (reach[source][destination] ? "YES" : "NO") << '\n';
    }
}
*/

/*
PROBLEM STATEMENT 1/2 (Related): The cost of a path is its largest edge, and each query asks for the smallest possible path cost.
PROBLEM STATEMENT 2/2: This is the minimax path problem, used for noise, danger, difficulty, or maximum-edge constraints.

SOLUTION:
Replace addition with max while keeping min outside the relaxation.

void solve_minimax_paths() {
    int V, E;
    cin >> V >> E;

    vector<vector<ll>> best(V, vector<ll>(V, BIG_INF));
    for (int i = 0; i < V; ++i) best[i][i] = 0;

    while (E--) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        best[u][v] = min(best[u][v], w);
        best[v][u] = min(best[v][u], w); // remove for directed graph
    }

    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                best[i][j] = min(best[i][j],
                                 max(best[i][k], best[k][j]));
            }
        }
    }

    int queryCount;
    cin >> queryCount;

    while (queryCount--) {
        int source, destination;
        cin >> source >> destination;
        cout << (best[source][destination] == BIG_INF
                     ? -1
                     : best[source][destination])
             << '\n';
    }
}
*/

/*
PROBLEM STATEMENT 1/2 (Related): Every edge has a capacity, and a path can carry only the minimum capacity on that path.
PROBLEM STATEMENT 2/2: For every query, maximize that minimum capacity; print -1 when no route exists.

SOLUTION:
Use maximum outside and minimum inside the Floyd-Warshall relaxation.

void solve_widest_paths() {
    int V, E;
    cin >> V >> E;

    vector<vector<ll>> capacity(V, vector<ll>(V, -1));
    for (int i = 0; i < V; ++i) capacity[i][i] = BIG_INF;

    while (E--) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        capacity[u][v] = max(capacity[u][v], w);
        capacity[v][u] = max(capacity[v][u], w); // remove for directed graph
    }

    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                capacity[i][j] = max(capacity[i][j],
                                     min(capacity[i][k], capacity[k][j]));
            }
        }
    }

    int queryCount;
    cin >> queryCount;

    while (queryCount--) {
        int source, destination;
        cin >> source >> destination;

        if (source == destination) cout << 0 << '\n';
        else cout << capacity[source][destination] << '\n';
    }
}
*/

/*
PROBLEM STATEMENT 1/2 (Related): Find the minimum total weight of any directed cycle in a non-negative weighted graph.
PROBLEM STATEMENT 2/2: Print -1 if the graph has no directed cycle, including no self-loop.

SOLUTION:
Initialize every value, including the diagonal, to INF. After Floyd-Warshall,
dist[i][i] is the cheapest non-empty cycle through i.

void solve_minimum_directed_cycle() {
    int V, E;
    cin >> V >> E;

    vector<vector<ll>> dist(V, vector<ll>(V, BIG_INF));

    while (E--) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        dist[u][v] = min(dist[u][v], w);
    }

    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            if (dist[i][k] == BIG_INF) continue;

            for (int j = 0; j < V; ++j) {
                if (dist[k][j] == BIG_INF) continue;
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    ll answer = BIG_INF;
    for (int i = 0; i < V; ++i) answer = min(answer, dist[i][i]);

    cout << (answer == BIG_INF ? -1 : answer) << '\n';
}
*/

/*
PROBLEM STATEMENT 1/2 (Related): Several people start in different cities and want to meet in one city with minimum total travel.
PROBLEM STATEMENT 2/2: Print the best meeting city and total distance, or -1 if no city is reachable by everyone.

SOLUTION:
Run normal Floyd-Warshall, try every city as the meeting point, and sum the
distance from every person's starting city.

void solve_best_meeting_city() {
    int V, E, peopleCount;
    cin >> V >> E >> peopleCount;

    vector<Edge> edges(E);
    for (Edge& edge : edges) cin >> edge.u >> edge.v >> edge.w;

    vector<int> people(peopleCount);
    for (int& city : people) cin >> city;

    vector<vector<ll>> dist = getDistances(V, edges, true);

    int bestCity = -1;
    ll bestTotal = BIG_INF;

    for (int meeting = 0; meeting < V; ++meeting) {
        ll total = 0;
        bool possible = true;

        for (int person : people) {
            if (dist[person][meeting] == BIG_INF) {
                possible = false;
                break;
            }
            total += dist[person][meeting];
        }

        if (possible && total < bestTotal) {
            bestTotal = total;
            bestCity = meeting;
        }
    }

    if (bestCity == -1) cout << -1 << '\n';
    else cout << bestCity << ' ' << bestTotal << '\n';
}
*/

