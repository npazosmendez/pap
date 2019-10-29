#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int,int> ii;

#define forr(i,a,b) for(int i=(a); i <(b); i++)
#define forn(i,n) forr(i,0,n)
#define debug(v) cerr << #v << ": " << (v) << endl;

#define forall(it,v) for(auto it = v.begin(); it != v.end(); it++)
#define dforn(i,n) for(int i = (n)-1; i >= 0; i--)
#define debugc(c) do{cerr << #c << ": "; forall(it, c) {cerr << *it << " ";} cerr << endl; } while(0);

#define operacion(x,y) (x+y)
const int nuetro_operacion = 0;
const int neutro_assign = -1;
const bool neutro_invertir = false;
struct SegmentTree {
    int sz;
    vector<int> t;
    vector<int> dirty_assign;
    vector<bool> dirty_invertir;
    SegmentTree(vector<int> &v) {
        sz = 1 << (32 - __builtin_clz((int)v.size()));
        t.assign(2*sz, nuetro_operacion);
        dirty_assign.assign(2*sz, neutro_assign);
        dirty_invertir.assign(2*sz, neutro_invertir);
        forn(i,v.size()) t[sz+i] = v[i];
        dforn(i, sz) t[i] = operacion(t[2*i], t[2*i + 1]);
    }

    void set_assign(int n, int val) {
        if(val == -1) return;
        dirty_assign[n] = val;
        // si asigné, me olvido de las inversiones
        // esto quiere decir que si hay una inversión, seguro vino DESPUÉS
        dirty_invertir[n] = neutro_invertir;
    }

    void set_invertir(int n, bool val) {
        if(val == neutro_invertir) return;
        dirty_invertir[n] = !dirty_invertir[n];
    }

    void update(int n, int a, int b) { // actualiza el valor del nodo "n" unicamente
        // si hay que invertir, hay que hacerlo después de asignar
        if (dirty_assign[n] != -1) t[n] = dirty_assign[n]*(b-a);
        if (dirty_invertir[n]) t[n] = (b-a)-t[n];
    }

    void push(int n, int a, int b) { // propaga los dirty hacia los hijos, actualizando el valor del nodo
        if (dirty_assign[n] != neutro_assign or dirty_invertir[n] != neutro_invertir) {
            // actualizo mi valor de la operacion
            update(n,a,b);
            if (n < sz) {
                // mando modificacion a mis hijos
                set_assign(2*n, dirty_assign[n]);
                set_assign(2*n + 1, dirty_assign[n]);

                set_invertir(2*n, dirty_invertir[n]);
                set_invertir(2*n + 1, dirty_invertir[n]);
            }
            // yo ya estoy limpio
            dirty_assign[n] = neutro_assign;
            dirty_invertir[n] = neutro_invertir;
        }
    }

    void invertir(int i, int j, int n, int a, int b) {
        push(n,a,b);
        if (j <= a || b <= i) return; // no interseca
        if (i <= a && b <= j) { // contenido completamente
            set_invertir(n, true);
            push(n,a,b);
            return;
        }
        // interseca parcialmente
        int mid = (a+b)/2;
        invertir(i, j, 2*n, a, mid);
        invertir(i, j, 2*n+1,mid,b);
        t[n] = operacion(t[2*n], t[2*n + 1]);
    }
    void invertir(int i, int j) { invertir(i,j,1,0,sz); }

    void assign(int val, int i, int j, int n, int a, int b) {
        push(n,a,b);
        if (j <= a || b <= i) return;
        if (i <= a && b <= j) {
            set_assign(n, val);
            push(n,a,b);
            return;
        }
        int mid = (a+b)/2;
        assign(val, i, j, 2*n, a, mid);
        assign(val, i, j, 2*n+1,mid,b);
        t[n] = operacion(t[2*n], t[2*n + 1]);
    }
    void assign(int val, int i, int j) { assign(val, i,j,1,0,sz); }

    // Query de consulta
    int get(int i, int j, int n, int a, int b) {
        if (j <= a || i >= b) return nuetro_operacion;
        push(n, a, b);
        if (i <= a && b <= j) return t[n];
        int c = (a+b)/2;
        return operacion(get(i, j, 2*n, a, c), get(i, j, 2*n+1, c, b));
    }
    int get(int i, int j) { return get(i, j, 1, 0, sz); }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int tt; cin >> tt;
    forn(t,tt){
        int M; cin >> M;
        string pirates;
        while(M--){
            int T; cin >> T;
            string subpirates; cin >> subpirates;
            while (T--) pirates += subpirates;
        }
        int n = pirates.size();
        vector<int> v(n);
        forn(i,n) v[i] = pirates[i]-'0';
        SegmentTree st(v);

        int Q; cin >> Q;
        cout << "Case " << t+1 << ":\n";
        int Scount = 0;
        forn(q,Q){
            char type; cin >> type;
            int a,b; cin >> a >> b;
            b++; // fuck
            switch (type){
            case 'F':
                // rango [a,b] a 1
                st.assign(1, a,b);
                break;
            case 'E':
                // rango [a,b] a 0
                st.assign(0, a,b);
                break;
            case 'I':
                st.invertir(a,b);
                // invertir rango [a,b]
                break;
            case 'S':
                // cuantos 1 en rango [a,b]
                Scount++;
                cout << "Q"<<Scount<<": "<< st.get(a,b) << "\n";
                break;
            default:
                assert(false);
            }
        }

    }
}