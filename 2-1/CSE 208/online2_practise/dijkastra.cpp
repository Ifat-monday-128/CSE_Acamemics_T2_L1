#include<bits/stdc++.h>
//#inc
using namespace std;
class edge{
    public:  
    int v;
    int w;
     edge(int v,int w){
         this->v=v;
         this->w=w;
     }
};
void dijkstra(int src,vector<vector<edge>> g,int v){
vector<int> dist(v,INT_MAX);
dist[src]=0;
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
int main(){
    int v=6;
    vector<vector<edge>> g(v);
    g[0].push_back(edge(1,2));
    g[0].push_back(edge(2,4));
    g[1].push_back(edge(2,1));
    g[1].push_back(edge(3,7));

    g[2].push_back(edge(3,3));
    g[2].push_back(edge(4,2));
    g[3].push_back(edge(5,1));
    g[4].push_back(edge(5,4));
    dijkstra(0,g,v);
    return 0;

}
//all operation in priority queue is log(n) and we are doing it for all edges so time complexity is O(ElogV) where E is number of edges and V is number of vertices.
//basically it is o((e+v)logv) but in sparce graph . so vlov+elov and in dense graph O(elogv ) is bigger that O(vlogv) so we can say O(ElogV) is time complexity of dijkstra algorithm.