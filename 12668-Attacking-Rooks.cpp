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


constexpr int noparent = -1;
    
/** 
 * Algoritmo de Dinitz (llamado usualmente dinics).  Observaciones:
 * - los nodos se numeran de 0 a n-1.  En caso en que se numeren de 1 a n, simplemente generar un flowgraph con n+1 vertices
 *   sin conectar nada al vertice 0.
 * 
 * COMPLEJIDAD
 * - Este algoritmo tiene complejidad O(n^2m), lo cual es mas que suficiente para competencias (salvo push relabel para grafos densos).
 * - En el caso en que todas las capacidades sean 1, cuesta O(min(m^1/2, n^2/3)m) lo cual es O(n^{2/3}m) para caso denso O(m^{3/2}) para ralos .
 * - Cuando el grafo es bipartito cuesta O(sqrt(n)m), lo cual matchea con el que se usa en competencia (de Hopcroft y Karp) y es
 * mejor que el de Berge.
 * 
 * NOTAS:
 * Esta implementacion es basica, separada en subtareas, sin mezclar nada para mostrar una implementacion casi directa.
 * (ver dinics-fast.cpp)
 */
class flowgraph {
    struct edge {
        long long from,         //vertice del que parte la arista; solo por simetria y para simplificar, se puede sacar
                  to,           //vertice al que llega la arista
                  capacity,     //capacidad de la arista
                  flow,         //flujo de la arista, el residual es capacity - flow
                  reverse;      //indice a la arista reversa en el grafo residual
    };
    template<class T>
    using digraph = vector<vector<T>>;
    
    digraph<edge> residual;

    inline edge& reverse(const edge& e) {
        return residual[e.to][e.reverse];
    }

    
    /** 
     * Computa el grafo de niveles usando BFS a partir del residual.
     * Retorna un grafo donde por cada vertice se tienen los indices de los vecinos
     * que pertenecen al level graph en el grafo residual.
     */
    digraph<int> find_level_graph(int source) {
        digraph<int> level_graph(residual.size());
        vector<int> level(residual.size(), noparent);
        deque<int> q(1, source);
        level[source] = 0;
        
        while(not q.empty()) {
            auto v = q.front(); q.pop_front();
            for(int idx = 0; idx < residual[v].size(); ++idx) {
                auto& e = residual[v][idx];
                if(e.flow == e.capacity) continue;
                if(level[e.to] == noparent) {
                    q.push_back(e.to);
                    level[e.to] = level[v] + 1;
                }
                if(level[e.to] > level[v]) level_graph[v].push_back(idx);
            }
        }
        return level_graph;
    }
    
    /** 
     * Aplica DFS para encontrar un augementing path mientras se busca el blocking flow.
     * Retorna el valor del augmenting path, que es valido cuando dicho valor es mayor a 0.
     * En parent e index deja anotada la arista con la que se llega a cada vertice hasta sink
     * Anula los dead-ends del level_graph cuando los encuentra.
     */
    long long find_augmenting_path(digraph<int>* level_graph, int from, int sink, vector<int>* parent, vector<int>* index) {
        while(not level_graph->at(from).empty()) {
            auto& e = residual[from][level_graph->at(from).back()];
            auto success = e.capacity-e.flow;
            if(success > 0 && e.to != sink) success = min(success, find_augmenting_path(level_graph, e.to, sink, parent, index));
            if(success == 0) {
                //arista saturada! o dead end!
                level_graph->at(from).pop_back();
                continue;
            }
            //camino encontrado.  Guardamos la informacion y retornamos el flujo
            parent->at(e.to) = e.from;
            index->at(e.to) = level_graph->at(from).back();
            return min(success, e.capacity - e.flow);
        }
        //no habia augmenting path
        return 0;
    }
    
    /**
     * Busca iterativamente los augmenting paths, aumentandolos hasta tener un blocking flow.  Retorna
     * el valor del flujo aumentado.
     * Requiere: que ninguna arista este en el level graph, ya que se calcula en esta etapa.
     */
    long long find_blocking_flow(int source, int sink) {
        auto level_graph = find_level_graph(source);
        
        vector<int> parent(residual.size(), noparent);
        vector<int> index(residual.size(), 0);
        long long ans=0, volume;
        
        while((volume = find_augmenting_path(&level_graph, source, sink, &parent, &index)) > 0) {
            for(int to = sink; parent[to] != noparent; to = parent[to]) {
                auto& e = residual[parent[to]][index[to]];
                e.flow += volume; 
                reverse(e).flow -= volume;
            }
            ans += volume;
        }
        return ans;
    }
    
    
public:
    flowgraph(int n) {
        residual.assign(n, vector<edge>());
    }
    
    void add_edge(int from, int to, long long capacity) {
        if(capacity <= 0) return;
        auto idxfrom = (int)residual[from].size(), idxto = (int)residual[to].size();
        residual[from].push_back(edge{from, to, capacity, 0, idxto});
        residual[to].push_back(edge{to, from, 0, 0, idxfrom});
    }
    
    /**
     * Carga en this el flujo maximo de source a sink.  Notar que this podria
     * tener un flujo precargado y lo modifica para tener el flujo maximo.
     * Retorna todo el flujo que se pudo agregar.
     */
    long long maxflow(int source, int sink) {
        long long res = 0, step;        
        while((step = find_blocking_flow(source, sink)) > 0) {
            res += step;
        }
        return res;
    }

    void print(ostream& out) {
        for(int f = 0; f < residual.size(); ++f) {
            cout << f << ": ";
            for(auto e : residual[f]) {
                auto& rev = reverse(e);
                out << "(" << e.from << "," << e.to << "," << e.capacity << "," << e.flow << ") {" 
                    << rev.from << "," << rev.to << ","  << rev.capacity << "," << rev.flow << "}  -  ";
            }
            cout << endl;
        }
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int N;
    while(cin >> N){
        vector<vector<char>> tablero(N, vector<char>(N));
        vector<vector<int>> subcols(N, vector<int>(N, -1));
        vector<vector<int>> subrows(N, vector<int>(N, -1));
        forn(i,N) forn(j,N) cin >> tablero[i][j];
        int gsize = 2*N*(N+1)+2;
        int t = gsize-1;
        flowgraph g(gsize);
        int node = 1;
        forn(i,N) {
            forn(j,N) {
                if(tablero[i][j] == 'X'){
                    g.add_edge(0, node, 1);
                    node++;
                }else{
                    subcols[i][j] = node;
                }
            }
            g.add_edge(0, node, 1);
            node++;
        }
        forn(j,N) {
            forn(i,N) {
                if(tablero[i][j] == 'X'){
                    g.add_edge(node, t, 1);
                    node++;
                }else{
                    subrows[i][j] = node;
                }
            }
            g.add_edge(node, t, 1);
            node++;
        }

        forn(i,N) forn(j,N) if(tablero[i][j] != 'X') {
            g.add_edge(subcols[i][j], subrows[i][j], 1);
        }
        cout << g.maxflow(0, t) << endl;
    }
}