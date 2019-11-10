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


const int INT_INF = -1;
constexpr char min_alphabet = '0';
constexpr char max_alphabet = '1';
//bool queries[1000];
const int root_state = 0;

struct ahoState {
  unordered_map<char, int> go_to;
  int parent;
  int fail;
  // Si bien usamos aristas de output, es necesario tener un vector ya que podría haber patrones repetidos.
  vector<int> words;
  int output;
  bool leaf;

  ahoState(int parent_= INT_INF) : parent(parent_), fail(0), output(INT_INF), leaf(false) {}
};

struct ahoCorasick {
  vector<ahoState> state_machine;
  ahoCorasick(const vector<string>&);
  ll find_keywords(const string&);
};

ahoCorasick::ahoCorasick(const vector<string>& keywords) {
  state_machine.emplace_back();

  for (int i = 0; i < sz(keywords); ++i) {
    int state_index = root_state;
    for (char c : keywords[i]) {
      if (state_machine[state_index].go_to.count(c) == 0) {
        state_machine[state_index].go_to[c] = sz(state_machine);
        state_machine.emplace_back(state_index);
      }
      state_index = state_machine[state_index].go_to[c];
    }
    state_machine[state_index].leaf = true;
    state_machine[state_index].words.push_back(i);
  }

  for (char c = min_alphabet; c <= max_alphabet; ++c) 
    if (state_machine[root_state].go_to.count(c) == 0)
      state_machine[root_state].go_to[c] = root_state;

  auto states = queue<int>();

  for (auto it = state_machine[root_state].go_to.begin(); it != state_machine[root_state].go_to.end(); ++it)
    if (it->second != root_state) states.push(it->second);

  while (states.size() != 0) {
    int state_index = states.front();
    states.pop();

    for (auto it = state_machine[state_index].go_to.begin(); it != state_machine[state_index].go_to.end(); ++it) {
      int fail_index = state_machine[state_index].fail;
      
      //Falla hasta encontrar un camino por c
      while(state_machine[fail_index].go_to.count(it->first) == 0) fail_index = state_machine[fail_index].fail;
      fail_index = state_machine[fail_index].go_to[it->first];
      state_machine[it->second].fail = fail_index;

      if (state_machine[fail_index].leaf) state_machine[it->second].output = fail_index;
      else state_machine[it->second].output = state_machine[fail_index].output;

      states.push(it->second);
    }
  }
}

ll ahoCorasick::find_keywords(const string& x) {
  //for(int i = 0; i < 1000; i++) queries[i] = false; 
  unordered_set<int> matches;

  int current_state = root_state;
  for (char c : x) {
    while (state_machine[current_state].go_to.count(c) == 0) current_state = state_machine[current_state].fail;
    current_state = state_machine[current_state].go_to[c];

    if (state_machine[current_state].leaf) for (auto word_id : state_machine[current_state].words) {
        //queries[word_id] = true;
        matches.insert(word_id);
    }

    int output_index = current_state;
    while (state_machine[output_index].output != INT_INF){
      output_index = state_machine[output_index].output;
      for (auto word_id : state_machine[output_index].words) {
          matches.insert(word_id);
          //queries[word_id] = true;
      }
    }
  }
  return matches.size();
  /*
  ll res = 0;
  for (int i = 0; i < 1000; ++i) if(queries[i]) res++;
  return res; 
  */
}



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
        vector<string> main_patterns, extra_patterns;
        ahoCorasick main_tree(main_patterns), extra_tree(extra_patterns);
        bool dirty = false;
        while(N--){
            string query; cin >> query;
            string w = query.substr(1, query.size()-1);
            w = decrypt(w, L);
            if(query[0] == '+') {
                extra_patterns.push_back(w);
                dirty = true;
            } else if(query[0] == '?') {
                if(dirty){
                    // arboles desactualizados, recomputo
                    if(extra_patterns.size() > 500) {
                        // concateno al main
                        main_patterns.insert( main_patterns.end(), extra_patterns.begin(), extra_patterns.end() );
                        extra_patterns.clear();
                        main_tree = ahoCorasick(main_patterns);
                    }
                    extra_tree = ahoCorasick(extra_patterns);
                }
                L = main_tree.find_keywords(w) + extra_tree.find_keywords(w);
                cout << L << "\n";
                dirty = false;
            } else {
                cerr << query << endl;
                exit(1);
            }
        }
    }
}