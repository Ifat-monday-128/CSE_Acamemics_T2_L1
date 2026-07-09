#include<bits/stdc++.h>
using namespace std;
class Graph{
    vector<vector<int>>g;
    public:
    void addVertex()
    {
        g.push_back(vector<int>());
    }
    void addEdge(int a,int b){
        g[a-1].push_back(b-1);
        g[b-1].push_back(a-1);

    }
    void printallelement(){
        for(int i=0;i<g.size();i++){
            for(int j=0;j<g[i].size();j++){
                cout<<g[i][j];
            }
            cout<<endl;
        }
    }
};
int main(){
    Graph G;
    G.addVertex();
    G.addVertex();
    G.addVertex();
    G.addEdge(1,2);
    G.printallelement();
}