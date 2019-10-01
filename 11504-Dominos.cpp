#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
typedef pair<ll,int> ii;

#define forr(i,a,b) for(int i=(a); i <(int)(b); i++)
#define forn(i,n) forr(i,0,n)
#define rep(n) forn(_,n)
#define debug(v) cerr << #v << ": " << (v) << endl;

int n,m;
vector< vector<int> > graph, graph_inv;
vector<bool> visited;
list<int> order;
int comps;
vector<int> comp;

void dfs(vector< vector<int> > &g, int v, bool get_order=false){
    if(visited[v]) return;
    comp[v] = comps;
    visited[v] = true;
    for(int n : g[v]) dfs(g, n, get_order);
    if(get_order) order.push_front(v);
}

void solve(){
    order.clear();
    visited.assign(n, false);
    comp.assign(n, -1);
    forn(v,n) dfs(graph, v, true);

    visited.assign(n, false);
    comps = 0;
    // marco y cuento las componentes
    for(int v : order) {
        if(!visited[v]){
            dfs(graph_inv, v);
            comps++;
        }
    }
    // me fijo cuales compoentes tienen grado de entrada mayor a 0 (desde otra componente)
    set<int> reachable_comps;
    forn(v, n) for(int n : graph[v])
        if(comp[n] != comp[v]) reachable_comps.insert(comp[n]);

    // tiro las fichas de domino de las componentes no alcanzables desde otras. el resto cae solo
    cout << comps - reachable_comps.size() << "\n";
}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int tt; cin >> tt;
    rep(tt){
        cin >> n >> m;
        graph.assign(n, vector<int>());
        graph_inv.assign(n, vector<int>());
        rep(m){
            int from, to; cin >> from >> to;
            from--; to--;
            graph[from].push_back(to);
            graph_inv[to].push_back(from);
        }
        solve();
    }
}