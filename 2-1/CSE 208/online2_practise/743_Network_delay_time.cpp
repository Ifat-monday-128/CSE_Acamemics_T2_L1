#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define pb push_back
#define tata "\n"

class Solution {
public:
int networkDelayTime(vector<vector<int>>& times, int n, int k){
    vector<int> dist(n, INT_MAX);
    dist[k-1] = 0;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({0, k-1});
    while(!pq.empty())
    {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();
        if (d > dist[u]) continue;
        for(auto &e : times){
            int from = e[0] - 1;
            int to = e[1] - 1;
            int w = e[2];
            if (from == u && dist[to] > dist[u] + w){
                dist[to] = dist[u] + w;
                pq.push({dist[to], to});
            }
        }
    }
    for(int i = 0; i < n; i++){
        if(dist[i] == INT_MAX) return -1;
    }
    return *max_element(dist.begin(), dist.end());
}
    // paste LeetCode function here
};

int main() {
    Solution sol;

    // manually create sample input here
    // cout << sol.functionName(...) << tata;

    return 0;
}