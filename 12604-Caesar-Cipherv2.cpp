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
#define debugc(c) do{cerr << #c << ": "; forall(it, c) {cerr << (int)(*it) << " ";} cerr << endl; } while(0);

vector<int> kmppre(string& t){ // r[i]: longest border of t[0,i)
    vector<int> r(t.size()+1);r[0]=-1;
    int j=-1;
    forr(i,0,t.size()){
        while(j>=0&&t[i]!=t[j])j=r[j];
        r[i+1]=++j;
    }
    return r;
}
vector<int> kmp(string& s, string& t){ // find t in s
    vector<int> matches;
    int j=0;vector<int> b=kmppre(t);
    forr(i,0,s.size()){
        while(j>=0&&s[i]!=t[j])j=b[j];
        if(++j==t.size()){
            matches.push_back(i-j+1);
            j=b[j];
        }
    }
    return matches;
}


string A, W, S;
int ord[256]; // el inverso de A

int offset(char from, char to){
    // offset circual desde from a to, usando el orden de ord
    return  ord[to] >= ord[from] ? ord[to] - ord[from] : (ord[to]+A.size()) - ord[from]; 

}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int N; cin >> N;
    forn(t,N){
        cin >> A >> W >> S;
        forn(i, sz(A)) ord[A[i]] = i;

        map<int,int> offset_match_count; // offset -> cantidad de matches

        if(W.size() == 1){
            // caso particular con el patron siendo 1 sola letra
            for(char c : S) offset_match_count[offset(c, W[0])]++;
        }else{
            // IDEA PRINCIPAL
            // en vez de trabajar con los string originales, me guardo la diferencia de cada char con el anterior
            // (tienen largo 1 menos porque el primer char no tiene anterior)
            // Esto hace que sin importar el offset de la encriptacion los steps del patrón aparezcan tal cual en los steps del texto
            string text_steps(sz(S)-1, -1);
            string word_steps(sz(W)-1, -1);

            for (size_t i = 1; i < sz(S); i++) text_steps[i-1] = offset(S[i-1], S[i]);
            for (size_t i = 1; i < sz(W); i++) word_steps[i-1] = offset(W[i-1], W[i]);

            // para saber a qué offset corresponde, comparo el char del principio del match contra el segundo del patron original
            // esto es porque mis "steps" arrancaban en el segundo caracter, así que mis "matches" en realidad matchean desde allí
            vector<int> matches = kmp(text_steps, word_steps);
            for(int i : matches) offset_match_count[offset(W[1], S[i+1])]++;
        }

        // filro los offsets que tuvieron 1 solo match
        vector<int> candidates;
        for(auto off : offset_match_count) if(off.second == 1) candidates.push_back(off.first);

        if(candidates.size()){
            // hay candidatos, pueden ser varios
            if(candidates.size() == 1)
                cout << "unique: ";
            else
                cout << "ambiguous: ";
            forn(i, sz(candidates)-1) cout << candidates[i] << " ";
            cout << candidates.back() << "\n";
        }else{
            cout << "no solution\n";
        }
    }
}