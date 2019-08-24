#include <vector>
#include <iostream>
#include <cassert>

using namespace std;

typedef unsigned long long ull;
ull n;
// TODO: dp

ull bishops(ull i, ull l, ull k){
        /*
             _|_|
           _|_|_|
         _|_|_|_|
        |_|_|_|_|
        |_|_|_|_|
          |_|_|_|
            |_|_|
              |_|
     i=  1 2 3 4 ...
        
        En cada columna puedo poner 0, 1 ó 2 alfiles (uno de cada lado del tablero).
        La última columna es la excepción, en donde puedo poner 1 ó 0.
        En cada columna que avanzo se me agregan 2 filas libres, que antes no existían.
        */

    // i: número de columna (mirando el tablero girado 45gr)
    // l: cantidad de filas libres
    // k: alfiles que restan poner en las columnas [i..n/2]
    if (k<0 or l<0) return 0; // combinación imposible, me pasé
    if (k==0) return 1; // no quedan bishops, tablero vacío único
    if (i==n) return k==1 ? l : 0; // llegue a la "columna" del medio, donde puedo poner un único alfil en "l" lugares
    if (i==n+1) return 0; // la columna anterior era la última y k>0, me sobraron
    return
        // como miro el tablero girado 45gr, en cada nueva "columna" se agregan 2 espacios libres
        bishops(i+2, l+2-1, k-1) * l * 2 + // pongo 1 acá. Tengo "l" formas. Como miro la mitad del tablero, tengo el doble de formas
        bishops(i+2, l+2-2, k-2) * (l*l-l)/2 * 2+ // pongo 2 acá. Tengo comb(l 2) formas. Como miro la mitad del tablero, tengo el doble de formas
        bishops(i+2, l+2, k); // no pongo ninguna acá
    
}

ull bishopsB(ull k) {return bishops(1, 1, k);}
ull bishopsN(ull k) {return bishops(2, 2, k);}

int main(int argc, char const *argv[])
{
    ull k;
    while(true){
        cin >> n >> k;
        if(n==0 &&  k==0) break;
        ull res = 0;
        for (size_t i = 0; i <= k; i++){
            res += bishopsB(i)*bishopsN(k-i);
        }
        cout << res << endl;
    }
    return 0;
}
