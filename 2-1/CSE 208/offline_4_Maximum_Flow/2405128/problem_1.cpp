#include <bits/stdc++.h>
using namespace std;
vector<vector<int>>residual;
vector<vector<int>>graph;
bool bfs(int s,int des,vector<int>&p,int n)
{
    queue<int>q;
    q.push(s);
    vector<int>visited(n,0);
    visited[s]=1;
    p[s]=-1;
    while(!q.empty())
    {
        int u=q.front();q.pop();
        for(auto v:graph[u])
        {
            if(!visited[v] && residual[u][v]>0)
            {
                visited[v]=1;
                p[v]=u;
                q.push(v);
                if(v==des)
                return true;
            }
        }
        
    }
    return false;
}
int E_karp(int s,int des,int n)
{
    int maxF=0;
    vector<int>p(n);
    while(bfs(s,des,p,n))
    {
        int pathF=INT_MAX;
        int v=des;
        while(v!=s)
        {
            int u=p[v];
            pathF=min(pathF,residual[u][v]);
            v=u;
        }
        v=des;
        while(v!=s)
        {
            int u=p[v];
            residual[u][v]-=pathF;
            residual[v][u]+=pathF;
            v=u;
        }
        maxF+=pathF;
        
    }
    return maxF;
}
int main() {
    
    int n,m;
    cin>>n>>m;
    
    residual.assign(n,vector<int>(n,0));
    graph.assign(n,vector<int>());
    for(int i=0;i<m;i++)
    {
        int u,v,w;
        cin>>u>>v>>w;
      
        residual[u][v]+=w;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    cout<<E_karp(0,n-1,n)<<endl;
    return 0;
}