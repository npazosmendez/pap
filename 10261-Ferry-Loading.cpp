#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>

using namespace std;

constexpr int MAXN = 100;
constexpr int INF = MAXN*100+1;

unordered_map<int, unordered_map<int, unordered_map<int, int>>> dp;

vector<int> lengths;

int knapsack(int n, int w1, int w2) {
    if(w1 < 0 or w2 < 0) return -INF;
    if(n == lengths.size()) return 0;
    if(!dp[n][w1].count(w2))
        dp[n][w1][w2] = max(max(knapsack(n+1, w1, w2 - lengths[n]), knapsack(n+1, w1 - lengths[n], w2)) + 1, 0);
    return dp[n][w1][w2];
}

void imprimir(int n, int w1, int w2, int target){
    if (target == 0) return;
    if(knapsack(n+1, w1, w2-lengths[n]) == target-1){
        cout << "port\n";
        imprimir(n+1, w1, w2-lengths[n], target-1);
    }else if(knapsack(n+1, w1-lengths[n], w2) == target-1){
        cout << "starboard\n";
        imprimir(n+1, w1-lengths[n], w2, target-1);
    }
}

int main(int argc, char const *argv[])
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int tt, w;
    cin >> tt;
    for (size_t t = 0; t < tt; t++)
    {
        cin >> w;
        w = w*100;
        int length = -1;
        lengths.clear();
        dp.clear();
        while(true){
            cin >> length;
            if(length == 0) break;
            lengths.push_back(length);
        }
        int res = knapsack(0, w, w);
        cout << res << '\n';
        imprimir(0,w,w, res);
        if(t < tt-1) cout << "\n";
    }
    return 0;
}