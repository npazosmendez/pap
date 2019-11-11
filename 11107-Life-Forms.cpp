#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int,int> ii;

#define forr(i,a,b) for(int i=(a); i <(b); i++)
#define forn(i,n) forr(i,0,n)
#define sz(v) (int)(v).size()
#define debug(v) cerr << #v << ": " << (v) << endl;

#define forall(it,v) for(auto it = v.begin(); it != v.end(); it++)
#define dforn(i,n) for(int i = (n)-1; i >= 0; i--)
#define zero(v) memset(v, 0, sizeof(v));
#define pb push_back
#define mp make_pair
#define debugc(c) do{cerr << #c << ": "; forall(it, c) {cerr << *it << " ";} cerr << endl; } while(0);

#define all(x) x.begin(), x.end()

struct SuffixArray {
	vector<int> sa, lcp;
	SuffixArray(basic_string<int>& s, int lim=256) {
		int n = sz(s) + 1, k = 0, a, b;
		vector<int> x(all(s)+1), y(n), ws(max(n, lim)), rank(n);
		sa = y;
		lcp = vector<int>(sz(s));
		iota(all(sa), 0);
		for (int j = 0, p = 0; p < n; j = max(1, j * 2), lim = p) {
			p = j, iota(all(y), n - j);
			forr(i,0,n) if (sa[i] >= j) y[p++] = sa[i] - j;
			fill(all(ws), 0);
			forr(i,0,n) ws[x[i]]++;
			forr(i,1,lim) ws[i] += ws[i - 1];
			for (int i = n; i--;) sa[--ws[x[y[i]]]] = y[i];
			swap(x, y), p = 1, x[sa[0]] = 0;
			forr(i,1,n) a = sa[i - 1], b = sa[i], x[b] =
				(y[a] == y[b] && y[a + j] == y[b + j]) ? p - 1 : p++;
		}
		forr(i,1,n) rank[sa[i]] = i;
		for (int i = 0, j; i < n - 1; lcp[rank[i++]-1] = k)
			for (k && k--, j = sa[rank[i] - 1];
					s[i + k] == s[j + k]; k++);
		
		sa.erase(sa.begin());
		lcp.erase(lcp.begin());
	}
};


struct entry{
	int len, s1, s2, suff_ix;
	entry(int len_, int s1_, int s2_, int suff_ix_) : 
		len(len_), s1(s1_), s2(s2_), suff_ix(suff_ix_) {}
};

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
	int t = 0;
    while(true){
        int n; cin >> n;
        if(n==0) break;
		if(t++ > 0) cout << "\n";
		int sentinel = 257;
		basic_string<int> s;
		vector<int> which;
		if(n==1){
            string dna; cin >> dna;
			cout << dna << "\n";
			continue;
		}
        forn(i, n){
			s.push_back(sentinel++);
			which.push_back(-1);
            string dna; cin >> dna;
			for(auto c: dna) s.push_back(c);
			forn(j, sz(dna)) which.push_back(i);
        }
		auto sa = SuffixArray(s, sentinel);

		vector<entry> lcp;
		forn(i, sz(sa.lcp)){
			int suffix_ix1 = sa.sa[i];
			int suffix_ix2 = sa.sa[i+1];
			lcp.push_back(entry(
				sa.lcp[i],
				which[suffix_ix1],
				which[suffix_ix2],
				suffix_ix1
			));
		}

		vector<int> seen_count(n, 0);
		int disting = 0;
		int l = 0, r = 0;
		int max_seen = -1;
		vector<int> indexes;
		st<min> st(sa.lcp);

		while(l < sz(lcp)){
			while(r < sz(lcp) and disting <= n/2){
				auto e = lcp[r++];
				if(e.s1 != -1) if(!seen_count[e.s1]++) disting++;
				if(e.s2 != -1) if(!seen_count[e.s2]++) disting++;
			}
			if(disting <= n/2){
				break;
			}
			int local = st.query(l,r);
			if(local > max_seen){
				max_seen = local;
				indexes.clear();
				indexes.push_back(lcp[r-1].suff_ix);
			} else if(local == max_seen){
				indexes.push_back(lcp[r-1].suff_ix);
			}
	
			auto e = lcp[l++];
			if(e.s1 != -1) if(!--seen_count[e.s1]) disting--;
			if(e.s2 != -1) if(!--seen_count[e.s2]) disting--;
		}
		if(max_seen){
			set<string> all;
			for(auto suffix_ix : indexes) {
				string substring(s.begin()+suffix_ix, s.begin()+suffix_ix+max_seen);
				all.insert(substring);
			}
			for(auto &ss : all) cout << ss << "\n";
		}else{
			cout << "?\n";
		}
    }
} 