/**
 * Author: Francisco Soulignac (modificado levemente por Dario y Nicolas)
 * Time in UVA: 0.31
 */

#include <bits/stdc++.h>

using namespace std;
using ll = long long;


class LCA {

    struct Query {
        int v, idx;
    };
    
    //pos[v] primera aparicion de v en un recorrido
    //dist[i] = distancia del vertice que se recorre en el i-esimo paso del DFS a la raiz (lo necesario para RMQ)
    vector<int> pos, uf; 
    vector<ll> dist;
    vector<vector<Query>> rmqs;
    int qsize = 0;
    
    //calcula la info para el rmq; notar que el arbol ya lo tenemos
    void dfs(const vector<vector<int>>& c, const vector<ll>& d, int v) {
        pos[v] = dist.size();
        dist.push_back(d[v]);
        for(auto w: c[v]) {
            dfs(c, d, w);
            dist.push_back(d[v]);
        }
    }
    
    int find(int v) {
        return uf[v] == -1 ? v : uf[v] = find(uf[v]);
    }
    
public:
    LCA(const vector<ll>& d, const vector<vector<int>>& c) : pos(d.size()), rmqs(2*d.size()), qsize(0) {
        dfs(c, d, 0);
    }
    
    void add_query(int v, int w) {
        if(pos[v] > pos[w]) swap(v, w);
        rmqs[pos[w]].push_back({pos[v], qsize++});
    }
    
    vector<ll> answers() {
        uf.assign(dist.size(), -1);
        vector<int> roots;
        vector<ll> res(qsize);
        for(int to = 0; to < dist.size(); ++to) {
            while(not roots.empty() and dist[roots.back()] > dist[to]) {
                uf[roots.back()] = to;
                roots.pop_back();
            }
            roots.push_back(to);
            for(auto q: rmqs[to]) {
                res[q.idx] = dist[q.v] + dist[to] - 2*dist[find(q.v)];
            }
        }
        return res;
    }
};


int main() {
    ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    
    int n, q;
    while(cin >> n and n) {
        //d = distancia a la raiz
        vector<ll> d(n, 0); 
        vector<vector<int>> T(n);
    
        for(int v = 1; v < n; ++v) {
            int u; ll l;
            cin >> u >> l;
            d[v] = d[u] + l;
            T[u].push_back(v);
        }

        LCA lca(d, T);
        cin >> q;
        while(q--) {
            int v, w; 
            cin >> v >> w;
            lca.add_query(v, w);
        }
        auto ds = lca.answers();
        for(int i = 0; i+1 < ds.size(); ++i) cout << ds[i] << ' ';
        if(not ds.empty()) cout << ds.back();
        cout << '\n';
    }
}
