#include <iostream>
#include <vector>

using namespace std;

constexpr int MAXN = 10000;
constexpr int INF = MAXN*100+1;
constexpr int MAXC = 100*100;

int mem[MAXN+1][MAXC+1];
vector<int> cost, benefit;

int knapsack(int n, int w) {
    if(w < 0) return -INF;
    if(n == -1) return 0;
    if(mem[n][w] < 0)
        mem[n][w] = max(knapsack(n-1, w), benefit[n] + knapsack(n-1, w - cost[n]));
    return mem[n][w];
}
void imprimir(int n, int target){
    if (n == -1) return;
    if(knapsack(n-1, target) == target){
        imprimir(n-1, target);
        cout << "port\n";
    }else{
        imprimir(n-1, target-benefit[n]);
        cout << "starboard\n";
    }
}

int main(int argc, char const *argv[])
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int tt, w, length, sum;
    cin >> tt;
    for (size_t t = 0; t < tt; t++)
    {
        cin >> w;
        w = w*100; // a centimetros
        sum = 0;
        cost.clear(); benefit.clear();
        while(true){
            cin >> length;
            if(length == 0) break;
            if (sum < w*2) { // solo me importan los autos que son potenciales candidatos a entrar
                sum += length;
                // podría user un solo vector, pero quiero hacer explícito el modelado con knapsack
                cost.push_back(length); benefit.push_back(length);
            }
        }

        for(int i = 0; i <= cost.size(); ++i) for(int j = 0; j <= w; ++j) mem[i][j] = -1;

        int n = cost.size()-1;
        while(n >=0 and sum > 2*knapsack(n, w)) { // mientras el lane más largo tenga menos de la mitad, pruebo tomar un auto menos
            sum -= cost[n];
            n--;
        }
        cout << n+1 << "\n";
        imprimir(n, knapsack(n, w));
        if(t < tt-1) cout << "\n";
    }
    return 0;
}