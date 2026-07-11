#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define pb push_back
#define tata "\n"
class edge{
    public:  
    int v;
    int w;
     edge(int v,int w){
         this->v=v;
         this->w=w;
     }
};
class Solution {
public:
   int minimumEffortPath(vector<vector<int>>& heights) {
        vector<vector<edge>> g;
        int n=heights.size();
    int v=n*n;
vector<int> dist(v,INT_MAX);
dist[0]=0;
for(int )
priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
pq.push({0,src});
while(!pq.empty())
{
int u=pq.top().second;
pq.pop();
for(auto e:g[u]){
    if(dist[e.v]>dist[u]+e.w){
        dist[e.v]=dist[u]+e.w;
        pq.push({dist[e.v],e.v});
    }
}
}
for(int i=0;i<v;i++){
    cout<<"Distance from source to vertex "<<i<<" is "<<dist[i]<<endl;
}}
    }};

int main() {
    Solution sol;

    // manually create sample input here
    // cout << sol.functionName(...) << tata;

    return 0;
}