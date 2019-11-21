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

const double EPS = 1e-14;
const double INF = 1e14;
struct pto {
    double x, y;
    pto(double x=0, double y=0) : x(x), y(y) {}
    pto operator-(pto a){return pto(x-a.x, y-a.y);}
    pto operator/(double a){return pto(x/a, y/a);}
    bool operator<(const pto &a) const {
        return (x < a.x - EPS) || (abs(x - a.x) < EPS && y < a.y-EPS);
    }
    double norm(){return sqrt(x*x + y*y);}
};


struct line{
	line() {}
	double a,b,c;//Ax+By=C
	line(double a, double b, double c):a(a),b(b),c(c){}
	line(pto p, pto q): a(q.y-p.y), b(p.x-q.x), c(a*p.x+b*p.y) {}
    pto inter(line l2){
        double det=a*l2.b-l2.a*b;
        if(abs(det)<EPS) return pto(INF, INF);
        return pto(l2.b*c-b*l2.c, a*l2.c-l2.a*c)/det;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int C; cin >> C;
    while(C--){
        vector<pto> puntos;
        int N; cin >> N;
        pto p;
        forn(i, N){
            cin >> p.x >> p.y;
            puntos.push_back(p);
        }
        sort(puntos.begin(), puntos.end());
        double res = 0;
        line sol(0, 1, 0); // la linea horizontal que representa hasta donde está tapado el sol
        // el "c" (tercer valor) es el "y"

        dforn(i, N) if(puntos[i].y > sol.c) { // si el pico pasa la altura del sol
            pto lo = puntos[i+1], hi = puntos[i]; // los extremos de este filo
            pto to = hi, from = sol.inter(line(lo, hi)); // los extremos de la parte iluminada del filo
            res += (to-from).norm();
            sol.c = hi.y;
        }
        printf("%.2f\n", res);
    }
}