// Solucion para https://www.spoj.com/problems/THRBL/ usando Sparse Table

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
        // assert(l < r);
        int k = 31 - __builtin_clz(r-l);
        return op(t[k][l], t[k][r-(1<<k)]);
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int N,M; cin >> N >> M;
    vector<int> H(N);
    forn(i,N) cin >> H[i];
    st<max> st(H);
    int res = 0;
    forn(i,M){
        int a,b; cin >> a >> b; a--; b--;
        if(st.query(min(a,b), max(a,b)) <= H[a]) res++;
    }
    cout << res << endl;
}