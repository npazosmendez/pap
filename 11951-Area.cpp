#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

using ll = long long int;
using ii = pair<ll,ll>;
using vi = vector<ii>;

#define area second
#define price first

constexpr ii MINUS_INF = make_pair(-(1ll << 62), -(1ll << 62));

ll K;

ii max(ii a, ii b){
    if (a.price > K and b.price > K) return make_pair(0, 0); // no puedo pagar ninguno
    if (a.price <= K and b.price > K) return a; // solo puedo pagar a
    if (a.price > K and b.price <= K) return b; // solo puedo pagar b
    // puedo pagar ambos
    if (a.area > b.area) return a; // a es de más area
    if (a.area < b.area) return b; // b es de más area
    return a.price < b.price ? a : b; // son de igual area, me quedo con el más barato 
}

ii add(ii a, ii b){
    return make_pair(a.price + b.price, b.area + a.area);
}

ii sub(ii a, ii b){
    return make_pair(a.price - b.price, a.area - b.area);
}

ii kadane(vi input) {
    ii ans, last;
    ans = last = input[0];
    int comienzo = 0;
    for(int i = 1; i < input.size(); ++i) {
        last = add(last, input[i]);
        while (comienzo < i and last.price > K){
            last = sub(last, input[comienzo]);
            comienzo++;
        }
        //last = max(input[i], add(last, input[i]));
        ans = max(ans, last);
    }
    return ans;
}

ii MP(vi input, vector<int>::iterator dbegin, vector<int>::iterator dend) {
    if(dend == dbegin + 1) return kadane(input);
    
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
    int t = 1;
    while(t <= T) {
        dims.assign(2, 0);
        cin >> dims[0] >> dims[1] >> K;
        paralelepipedo.resize(dims[0] * dims[1]);
        for(int i = 0; i < dims[0]; ++i) for(int j = 0; j < dims[1]; ++j){
            ll P; cin >> P;
            paralelepipedo[(i * dims[1]) + j] = make_pair(P, 1);

        }
        ii res = MP(paralelepipedo, dims.begin(), dims.end());
        printf("Case #%d: %lld %lld\n", t, res.area, res.price);
        //if(T > 0) cout << '\n'; //que estupidez!
        t++;
    }
    
    return 0;
}