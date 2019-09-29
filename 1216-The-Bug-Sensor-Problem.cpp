#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
typedef pair<ll,int> ii;

#define forr(i,a,b) for(int i=(a); i <(int)(b); i++)
#define forn(i,n) forr(i,0,n)
#define rep(n) forn(_,n)
#define debug(v) cerr << #v << ": " << (v) << endl;

int rt;
int nodos;

struct UnionFind {
    vector<int> f; vector<int> _tam;
    void init(int sz) { f.assign(sz, -1); _tam.assign(sz, 1); }
    int comp(int x){ return (f[x] == -1 ? x : f[x] = comp(f[x])); }
    int tam(int x) { return _tam[comp(x)]; }
    bool join(int a, int b) {
        bool same = comp(a) == comp(b);
        if(!same) {
            if (tam(a) > tam(b))
                swap(a, b);

            _tam[comp(b)] += _tam[comp(a)];
            _tam[comp(a)] = 0;
            f[comp(a)] = comp(b);
        }
        return same;
    }
};

vector<pair<double, ii> > ejes;
double kruskal() {
    UnionFind uf;
    vector<pair<double, ii> > ejesRes;
    sort(ejes.begin(), ejes.end());
    uf.init(nodos);
    double peso = 0;
    for (auto eje : ejes) {
        // quiero el bosque generador al que le falten "rt" (#transmisores) ejes para ser árbol generador
        if((int)ejesRes.size() == nodos-rt) return peso;
        peso = eje.first;
        int a = eje.second.first;
        int b = eje.second.second;
        if (!uf.join(a, b)) {
            ejesRes.push_back({peso, {a,b}});
        }
    }
    return peso;
}


int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int w = 1; cin >> w;
    rep(w){
        cin >> rt;
        nodos = 0;
        vector<double> xx;
        vector<double> yy;
        ejes.clear();
        while(1){
            double x,y;
            cin >> x;
            if(x == -1) break; // molestísimo
            cin >> y;
            forn(i, nodos) {
                double dist = sqrt((x-xx[i])*(x-xx[i])+ (y-yy[i])*(y-yy[i]));
                ejes.push_back({dist, {nodos, i}});
            }
            xx.push_back(x);
            yy.push_back(y);
            nodos++;
        }
        cout << ceil(kruskal()) << "\n";
    }

}