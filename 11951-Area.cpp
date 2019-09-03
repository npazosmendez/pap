#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

using ll = long long int;
using ii = pair<ll,ll>;
using vi = vector<ii>;

#define area second
#define price first

ll K;

// el max range ahora trabaja con tuplas (area, precio)
// defino estas operaciones para modificar lo menos posible el algoritmo original
ii add(ii a, ii b){ return make_pair(a.price + b.price, b.area + a.area);}
ii sub(ii a, ii b){ return make_pair(a.price - b.price, a.area - b.area);}
ii max(ii a, ii b){
    if (a.area > b.area) return a; // a es de más area
    if (a.area < b.area) return b; // b es de más area
    return a.price < b.price ? a : b; // son de igual area, me quedo con el más barato 
}

ii ex_kadane(vi input) {
    // antes era kadane. ahora no funciona exactamente la misma idea, ya que queremos
    // el rango de suma máxima pero cuyo precio total no excede K
    ii ans, last;
    ans = last = input[0].price > K ? make_pair(ll(0),ll(0)) : input[0];
    int comienzo = input[0].price > K ? 1 : 0;
    for(int i = 1; i < input.size(); ++i) {
        last = add(last, input[i]);
        while (last.price > K){ // avanzo el comienzo del anterior hasta que me alcance la plata
            last = sub(last, input[comienzo]);
            comienzo++;
        }
        ans = max(ans, last);
    }
    return ans;
}

ii MP(vi input, vector<int>::iterator dbegin, vector<int>::iterator dend) {
    if(dend == dbegin + 1) return ex_kadane(input);
    
    int DIM = accumulate(dbegin+1, dend, 1, multiplies<int>());
    for(int i = 1; i < *dbegin; ++i) for(int j = 0; j < DIM; ++j) 
            input[i*DIM + j] = add(input[i*DIM + j], input[(i-1)*DIM + j]);
    
    vi input2;
    input2.resize(DIM);
    ii ans = make_pair(0,0);
    for(int i1 = 0; i1 < *dbegin; ++i1) for(int i2 = i1; i2 < *dbegin; i2++) {
        for(int j = 0; j < DIM; ++j) {
            input2[j] = sub(input[(i2*DIM)+j], (i1 > 0 ? input[(i1-1)*DIM+j] : make_pair(ll(0),ll(0))));
        }
        ans = max(ans, MP(input2, dbegin+1, dend));
    }
    return ans;
}

int main() {
    
    ios::sync_with_stdio(false);
    cin.tie(0);

    int T;
    vector<int> dims;
    vi paralelepipedo;

    cin >> T;
    for(int t = 1; t <= T; t++) {
        dims.assign(2, 0);
        cin >> dims[0] >> dims[1] >> K;
        paralelepipedo.resize(dims[0] * dims[1]);
        for(int i = 0; i < dims[0]; ++i) for(int j = 0; j < dims[1]; ++j){
            ll P; cin >> P;
            paralelepipedo[(i * dims[1]) + j] = make_pair(P, 1);
        }
        ii res = MP(paralelepipedo, dims.begin(), dims.end());
        printf("Case #%d: %lld %lld\n", t, res.area, res.price);
    }
    return 0;
}