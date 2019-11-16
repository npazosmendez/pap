#include <bits/stdc++.h>
/*
https://codeforces.com/group/YjFmW2O15Q/contest/101919/attachments
TAP 2018 "Estacionamiento"
*/
using namespace std;

typedef long long ll;
typedef pair<int,int> ii;

#define forr(i,a,b) for(int i=(a); i <(b); i++)
#define forn(i,n) forr(i,0,n)
#define debug(v) cerr << #v << ": " << (v) << endl;

#define forall(it,v) for(auto it = v.begin(); it != v.end(); it++)
#define dforn(i,n) for(int i = (n)-1; i >= 0; i--)
#define debugc(c) do{cerr << #c << ": "; forall(it, c) {cerr << *it << " ";} cerr << endl; } while(0);

int m;
vector<double> izq, der;
vector<double> izq_acum, der_acum;
const int MAXM = 1000;

double dp[MAXM+1][MAXM+1][2];

double minmean(int i, int d, int lado_actual){
    /*
    La "esperanza" de la longitud recorrida hasta encontrar el auto
    recorriendo las cuadras que restan de forma óptima (minimizando la "esperanza")
    donde:
        - i: cantidad de cuadras de la izquierda que ya recorrí
        - d: cantidad de cuadras de la derecha que ya recorrí
        - lado_actual: en donde estoy parado ahora mismo (0=izquierda, 1=derecha)
    
    Notar que:
    - no es una "esperanza" exactamente, porque las cuadras que restar por recorrer
    podrían no ser una distribución de probabilidad (podrían no sumar 1). Dado un
    recorrido R=cuadra_0, ...cuadra_2m, la esperanza para el mismo es
    sum(dist_hasta[cuadra_i]*proba[cuadra_i]). Lo que hacemos acá es calcular
    una parte (algo como un sufijo) de esta sumatoria.
    - la posición actual la podemos considerar determinada por "lado_actual" porque
    suponemos que en cada movimiento se explora exactamente 1 cuadra nueva, por lo
    que siempre estoy parado en uno de los dos extremos.
    */

    if(i==m and d==m) return 0;
    if(dp[i][d][lado_actual] != -1) return dp[i][d][lado_actual]; 

    // pruebo ir a izquierda
    double res_i = MAXM*MAXM;
    if(i<m){
        // la distancia que tengo que hacer para recorrer la próxima cuadra de la izquierda
        int dst_izq = lado_actual ? 1 : i+d+1;

        // La "esperanza" de la recursión (sobre las cuadras de más
        // allá de i+1 a izquierda y d a derecha) está calculada con
        // distancias medidas desde donde me voy a para ahora, no sabe
        // cuánto se caminó antes. Debo corregirlo:
        /*
            recursión = dist_1*proba_1 + ... + dist_n*proba_n
            lo que quiero = (dist_1 + rec_hasta_ahora)*proba_1 + ... + (dist_n+rec_hasta_ahora)*proba_n
                          = recursión + rec_hasta_ahora*(proba_1+...+proba_n)
        */
        double sum_prob = izq_acum[i+1] + der_acum[d];
        res_i = minmean(i+1,d, 1) + sum_prob*dst_izq + dst_izq*izq[i];
    }

    // pruebo ir a derecha
    double res_d= MAXM*MAXM;
    if (d<m){
        int dst_der = lado_actual ? i+d+1 : 1;
        double sum_prob = izq_acum[i]+der_acum[d+1];
        res_d = minmean(i,d+1, 0) + sum_prob*dst_der + dst_der*der[d];
    }

    // me quedo con el mínimo
    return dp[i][d][lado_actual] = min(res_d, res_i);
}

int main() {
    ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    cin >> m;
    izq.resize(m), der.resize(m);
    forn(i, m) cin >> izq[m-1-i];
    forn(i, m) cin >> der[i];

    // probas acumuladas: en "i" tengo sum(i...m)
    izq_acum = izq; izq_acum.push_back(0);
    der_acum = der; der_acum.push_back(0);
    dforn(i, m) izq_acum[i] += izq_acum[i+1];
    dforn(i, m) der_acum[i] += der_acum[i+1];

    // init dp
    forn(i, m+1) forn(j, m+1) dp[i][j][0] =-1, dp[i][j][1] = -1;

    printf("%.6f\n", minmean(0,0,0));
}