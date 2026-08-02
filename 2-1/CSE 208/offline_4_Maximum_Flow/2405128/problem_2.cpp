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
struct air_flight
{
    string id,from,to;
    int depart,arrive;
};
bool can_follow(air_flight a,air_flight b){
    bool same=(a.to==b.from);
    bool possible=(b.depart-a.arrive>=180);
    return (same && possible);
}
void edge_adding(int u,int v,int c)
{
    residual[u][v]+=c;
    graph[u].push_back(v);
    graph[v].push_back(u);
}
int main(){
    int n;
    cin>>n;
    vector<air_flight>flight(n);
    for(int i=0;i<n;i++){
        string departure,arrival;
        cin>>flight[i].id>>flight[i].from>>flight[i].to;
        cin>>departure>>arrival;
        int time1= stoi(departure.substr(0,2))*60+stoi(departure.substr(3,2));
        int time2= stoi(arrival.substr(0,2))*60+stoi(arrival.substr(3,2));
        flight[i].depart=time1;
        flight[i].arrive=time2;

        
    }
    int source=0;
    int sink=2*n+1;
    
     residual.assign(2*n+2,vector<int>(2*n+2,0));
    graph.assign(2*n+2,vector<int>());
    for(int i=0;i<n;i++){  //conecting sourc to the left part
        edge_adding(source,i+1,1);
    }
    for(int i=0;i<n;i++){ //connecting right to the src
        edge_adding(n+1+i,sink,1);
    }
     for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
        if(can_follow(flight[i], flight[j]))
            {
                int left=i+1;
                int right=n+j+1;

                edge_adding(left,right, 1);
            }
        }
    }
    int maxcount=E_karp(source,sink,2*n+2);
    int num_of_airp=n-maxcount;
    cout << "Number of Aircraft: " << num_of_airp << endl;

vector<int> next_F(n, -1);
vector<bool> prev_F(n, false);


for (int i = 0; i < n; i++)
{
    int left=i+1;

    for (int j=0;j<n;j++)
    {
        int right=n+1+j;

        if (residual[right][left]==1)
        {
            next_F[i] = j;
            prev_F[j] = true;
            break;
        }
    }
}


int air_count=1;

for (int i= 0;i<n;i++)
{
    if (prev_F[i]==false)
    {
        cout << "Aircraft "<<air_count<<": ";
        air_count++;
        int ongoing =i;

        while(ongoing!=-1)
        {
            cout<<flight[ongoing].id;

            if(next_F[ongoing]!=-1)
                cout<< " -> ";

            ongoing = next_F[ongoing];
        }

        cout << endl;
    }
}

return 0;


}