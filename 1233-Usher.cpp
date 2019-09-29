#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
typedef pair<ll,int> ii;

#define forr(i,a,b) for(int i=(a); i <(int)(b); i++)
#define forn(i,n) forr(i,0,n)
#define rep(n) forn(_,n)
#define debug(v) cerr << #v << ": " << (v) << endl;

int b, p, q;
vector< vector<ii> > graph;
const ll INF = 1e15 + 5;

ll dijkstra() {
    vector<ll> dist(graph.size(), INF);
    priority_queue<ii, vector<ii>, greater<ii>> Q;
    Q.push({0, 0});
    while(not Q.empty()) {
        auto u = Q.top();
        Q.pop();
        if(dist[u.second] < INF) continue;
        dist[u.second] = u.first;
        for(auto v : graph[u.second]) if(dist[v.second] == INF) {
            Q.push({u.first + v.first, v.second});
        }
    }
    return dist[p+1];
}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int c = 1; cin >> c;
    rep(c){
        cin >> b >> p >> q;
        graph.assign(p+2, vector<ii>()); // el usher, los p wachines, +1 nodo especial a distancia 0 del usher
        rep(q){
            int i; cin >> i;
            graph[0].push_back({0, i});
        }
        forn(i, p){
            int rules; cin >> rules;
            rep(rules){
                int coins, receiver; cin >> coins >> receiver;
                if(receiver == 0) receiver = p+1;
                graph[i+1].push_back({coins, receiver});
            }
        }
        graph[p+1].push_back({0, 0});
        ll min_dist = dijkstra();
        ll box = min_dist, acum = 0;
        while(box < b){
            acum++; box--;
            box += min_dist;
        }
        cout << acum << "\n";
    }

}