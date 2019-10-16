// Solucion para Ants Colony usando LCA
// https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=3390

#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

#define forr(i,a,b) for(int i=(a); i <(b); i++)
#define forn(i,n) forr(i,0,n)
#define sz(v) (int)(v).size()

struct LCA {
    vector<int> tour, heights, first;
    vector<vector<int>> table;
    #define lower(a, b) heights[a] < heights[b] ? a : b;

    LCA(vector<vector<int>> &G, int root = 0) {
        int n = sz(G);
        heights.assign(n, -1);
        first.resize(n);
        dfs(G, root);

        // sparse table
        int N = sz(tour);
        int K = 31 - __builtin_clz(N)+1;
        table.assign(K, vector<int>(N));
        forn(i,N) table[0][i] = tour[i];
        forr(k,1,K) forn(i, N-(1<<k)+1)
            table[k][i] = lower(table[k-1][i], table[k-1][i+(1<<(k-1))]);
    }

    void dfs(vector<vector<int>> &G, int v = 0, int h = 0) {
        if(heights[v] != -1) return;
        heights[v] = h;
        first[v] = (int)tour.size();
        tour.push_back(v);
        for (int w : G[v]) {
            dfs(G, w, h+1);
            tour.push_back(v);
        }
    }

    int query(int v, int w) {
        int left = first[v];
        int right = first[w];
        if (left > right) swap(left, right);
        int k = 31 - __builtin_clz(right-left);
        return lower(table[k][left], table[k][right-(1<<k)]);
    }
};


int main() {
    ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

    int n, qn;
    while(cin >> n and n) {
        //d = distancia a la raiz
        vector<ll> d(n, 0);
        vector<vector<int>> T(n);

        for (int v = 1; v < n; v++) {
            int u; ll l;
            cin >> u >> l;
            T[u].push_back(v);
            d[v] = d[u] + l;
        }

        LCA lca(T);
        cin >> qn;
        forn(qi, qn) {
            int v; cin >> v;
            int w; cin >> w;
            int anc = lca.query(v, w);
            cout << d[v] + d[w] - 2 * d[anc];
            if (qi != qn-1) cout << " ";
        }
        cout << "\n";
    }
}