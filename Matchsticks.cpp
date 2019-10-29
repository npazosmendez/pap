#include <bits/stdc++.h>

using namespace std;

#define forr(i,a,b) for(int i=(a); i <(b); i++)
#define forn(i,n) forr(i,0,n)
#define sz(v) (int)(v).size()


typedef const int&(*op)(const int&, const int&);  // asociativa, idempotente
template<op op>
struct st{
    vector<vector<int>> t;
    st(vector<int> &A) { // O(n.logn)
        int n = sz(A);
        int K = 31 - __builtin_clz(n)+1;
        t.assign(K, vector<int>(n));
        forn(i,n) t[0][i] = A[i];
        forr(k,1,K) forn(i, n-(1<<k)+1)
            t[k][i] = op(t[k-1][i], t[k-1][i+(1<<(k-1))]);
    }
    int query(int l, int r) {  // O(1), [l, r)
        assert(l < r);
        int k = 31 - __builtin_clz(r-l);
        return op(t[k][l], t[k][r-(1<<k)]);
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

    int n, q; cin >> n;
    vector<int> m(n);
    forn(i,n) cin >> m[i];
    cin >> q;
    st<max> st_max(m);
    st<min> st_min(m);
    forn(i,q){
        int l,r; cin >> l >> r;
        r++;
        double base_time = st_min.query(l,r);
        double extra_out_time = max(
            0 < l ? st_max.query(0, l) : 0,
            r < n ? st_max.query(r, n) : 0);
        double extra_in_time =  (st_max.query(l, r)-base_time)/2.0;
        printf("%.1lf\n", base_time + max(extra_in_time, extra_out_time));
    }
}