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

#define trav(e,c) for(auto &e : c)
struct AhoCorasick {
	enum {alpha = 2, first = '0'};
	struct Node {
		// (nmatches is optional)
		int back, next[alpha], start = -1, end = -1, nmatches = 0;
		Node(int v) { memset(next, v, sizeof(next)); }
	};
	vector<Node> N;
	vector<int> backp;
	void insert(string& s, int j) {
		assert(!s.empty());
		int n = 0;
		trav(c, s) {
			int& m = N[n].next[c - first];
			if (m == -1) { n = m = sz(N); N.emplace_back(-1); }
			else n = m;
		}
		if (N[n].end == -1) N[n].start = j;
		backp.push_back(N[n].end);
		N[n].end = j;
		N[n].nmatches++;
	}
	AhoCorasick(vector<string>& pat) {
		N.emplace_back(-1);
		forr(i,0,sz(pat)) insert(pat[i], i);
		N[0].back = sz(N);
		N.emplace_back(0);

		queue<int> q;
		for (q.push(0); !q.empty(); q.pop()) {
			int n = q.front(), prev = N[n].back;
			forr(i,0,alpha) {
				int &ed = N[n].next[i], y = N[prev].next[i];
				if (ed == -1) ed = y;
				else {
					N[ed].back = y;
					(N[ed].end == -1 ? N[ed].end : backp[N[ed].start])
						= N[y].end;
					N[ed].nmatches += N[y].nmatches;
					q.push(ed);
				}
			}
		}
	}
	ll count(string &word) {
		int n = 0;
		ll count = 0;
		trav(c, word) {
			n = N[n].next[c - first];
			count += N[n].nmatches;
		}
		return count;
	}
};


string decrypt(string& w, ll L){
    L = L % sz(w);
    string res = w.substr(L, w.size()-L) + w.substr(0, L);
    return res;
}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int T; cin >> T;
    forn(t, T){
        cout << "Case #" << t+1 << ":\n";
        int N; cin >> N;
        ll L = 0;
        // los patrones de cada arbol
        vector<string> main_patterns, extra_patterns;
        
        // los arboles
        AhoCorasick main_tree(main_patterns), extra_tree(extra_patterns);
        
        // trackeo los patrones ya vistos, así no agrego duplicados
        unordered_set<string> known_patterns;
        bool dirty = false;
        while(N--){
            string query; cin >> query;
            string w = query.substr(1, query.size()-1);
            w = decrypt(w, L);
            if(query[0] == '+') {
                if(known_patterns.count(w)) continue;
                known_patterns.insert(w);
                extra_patterns.push_back(w);
                dirty = true;
            } else if(query[0] == '?') {
                if(dirty){ // recomputar arboles!
                    if(extra_patterns.size() > 70) {
                        // vuelco todo al main
                        main_patterns.insert( main_patterns.end(), extra_patterns.begin(), extra_patterns.end() );
                        extra_patterns.clear();
                        main_tree = AhoCorasick(main_patterns);
                    }
                    extra_tree = AhoCorasick(extra_patterns);
                }
                L = main_tree.count(w) + extra_tree.count(w);
                cout << L << "\n";
                dirty = false;
            } else {
                cerr << query << endl;
                exit(1);
            }
        }
    }
}