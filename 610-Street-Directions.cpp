#include <bits/stdc++.h>

using namespace std;
typedef pair<int,int> ii;

#define forr(i,a,b) for(int i=(a); i <(int)(b); i++)
#define forn(i,n) forr(i,0,n)
#define rep(n) forn(_,n)
#define debug(v) cerr << #v << ": " << (v) << endl;

int n, m;
vector<vector<int>> graph;
vector<int> depth, low, color;

int dfs(int v, int padre, int d){
    depth[v] = d;
    low[v] = d;
    color[v] = 1;
    for(int w : graph[v]) if (w != padre){
        // si es la primera vez que veo la arista, la imprimo
        if(color[w] != 2) cout << v+1 << " " << w+1 << "\n";
        if(depth[w] == -1){
            low[v] = min(low[v], dfs(w,v,d+1));
            if(low[w] >= depth[w]){
                // Como (v-w) puente, tengo que poder volver de w al padre v
                cout << w+1 << " " << v+1 << "\n";
            }
        }else{
            low[v] = min(low[v], depth[w]);
        }
    }
    color[v] = 2;
    return low[v];
}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int t = 1;
    while(1){
        cin >> n >> m;
        if(n == m and n == 0) return 0;
        graph.assign(n, vector<int>());
        depth.assign(n, -1);
        low.assign(n, -1);
        color.assign(n,-1);
        rep(m){
            int i,j; cin >> i >> j; j--;i--;
            graph[i].push_back(j);
            graph[j].push_back(i);
        }
        cout << t << "\n\n";
        dfs(0,-1,0);
        cout << "#\n";
        t++;
    }

}