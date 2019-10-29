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

// copypaste de kmp
vector<string::size_type> compute_pi(const string &w){
    auto t = vector<string::size_type>(w.size());

    t[0] = 0;
    string::size_type len;

    for (string::size_type i = 1; i < w.size(); ++i){
        len = t[i - 1];
        while (len > 0 and w[len] != w[i]){
            len = t[len - 1];
        }
        if (w[i] == w[len]){
            len++;
        }
        t[i] = len;
    }

    return t;
}

// copypaste de kmp, me devuelve los índices en donde matchea el patrón
vector<string::size_type> kmp_search(const string &text, const string &pattern){
    vector<string::size_type> r;

    auto pi = compute_pi(pattern);

    string::size_type len = 0;

    for (string::size_type i = 0; i < text.size(); ++i){
        while (len > 0 and pattern[len] != text[i]){
            len = pi[len - 1];
        }
        if (text[i] == pattern[len]){
            len++;
            if (len == pattern.size()){
                r.push_back(i - (len - 1));
            }
        }
    }
    return r;
}

string rotate(string &s, int offset, string &A, map<char, int> &ord){
    // rota el string "s" un offset, utilizando el alfabeto A.
    // "ord" es simplemente el inverso del alfabeto, para saber una ubicación de un caracter rapidamente
    string res(s);
    forn(i, sz(s)) res[i] = A[(ord[s[i]] + offset) % sz(A)];
    return res;
}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int N; cin >> N;
    forn(t,N){
        string A, W, S;
        cin >> A >> W >> S;
        map<char, int> ord; // el inverso de A
        forn(i, sz(A)) ord[A[i]] = i;
        vector<int> matches;
        forn(offset, sz(A)){
            // pruebo este offset
            auto this_match = kmp_search(S, rotate(W, offset, A, ord));
            // si tuvo 1 match exacto, es un canidato
            if(this_match.size() == 1) matches.push_back(offset);
        }
        
        if(matches.size()){
            // hay candidatos, pueden ser varios
            if(matches.size() == 1)
                cout << "unique: ";
            else
                cout << "ambiguous: ";
            forn(i, sz(matches)-1) cout << matches[i] << " ";
            cout << matches.back() << "\n";
        }else{
            cout << "no solution\n";
        }
    }
}