#include <vector>
#include <iostream>
#include <cassert>
#include <unordered_map>

using namespace std;

typedef long long ll;
ll n;

// n, i, l, k -> result
unordered_map<int, unordered_map<int, unordered_map<int, unordered_map<int, ll>>>> dp;

ll bishops(ll i, ll l, ll k){
    /*
      Tablero visto a 45 grados y doblado al medio.
      La primera columna es 1 casilla negra, luego 2 blancas, luego 3 negras, etc.
           __
        __|NN|
     __|  |__|
    |NN|__|NN|
    |__|  |__|
       |__|NN|
          |__|
  i= 1  2  3 ...

      // i [1...n]: número de columna
      // l [0...n]: cantidad de filas libres
      // k: [0...n^2] alfiles que restan poner en las columnas [i..n/2]
    */
    if (k<0 or l<0) return 0; // combinación imposible, me pasé
    if (k==0) return 1; // no quedan bishops, tablero vacío único
    if (i==n) return k==1 ? l : 0; // llegue a la "columna" del medio, donde puedo poner un único alfil en "l" lugares
    if (i==n+1) return 0; // la columna anterior era la última y k>0, me sobraron
    if (!dp[n][i][l].count(k)) {
      dp[n][i][l][k] =
          // como miro el tablero girado 45gr, en cada nueva "columna" se agregan 2 espacios libres
          bishops(i+2, l+2-1, k-1) * l * 2 + // pongo 1 acá. Tengo "l" formas. Como miro la mitad del tablero, tengo el doble de formas
          bishops(i+2, l+2-2, k-2) * (l*l-l)/2 * 2+ // pongo 2 acá. Tengo comb(l 2) formas. Como miro la mitad del tablero, tengo el doble de formas
          bishops(i+2, l+2, k); // no pongo ninguna acá
    }
    return dp[n][i][l][k];
}

ll bishopsB(ll k) {return bishops(1, 1, k);}
ll bishopsN(ll k) {return bishops(2, 2, k);}

int main(int argc, char const *argv[])
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    ll k;
    while(true){
        cin >> n >> k;
        if(n==0 &&  k==0) break;
        ll res = 0;
        if(k < 2*n ){ // optimización: no puedo poner más de 2*n alfiles
          for (size_t i = 0; i <= k; i++){
              res += bishopsB(i)*bishopsN(k-i);
          }
        }
        cout << res << '\n';
    }
    return 0;
}
