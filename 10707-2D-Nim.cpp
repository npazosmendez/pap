#include <bits/stdc++.h>

using namespace std;
typedef pair<int,int> ii;

#define forr(i,a,b) for(int i=(a); i <(int)(b); i++)
#define forn(i,n) forr(i,0,n)

int t, W,H, n;
vector< vector<bool> > board1, board2;

enum color{blanco, gris, negro};
const vector<pair<int,int>> vecindad = {{1,0}, {-1,0}, {0,-1},{0,1}};

vector< vector<color> > dfs_board;

void dfs_visit(vector<vector<bool>> &board, int i, int j, list<ii> &componente){
    if(i < 0 or i >= W or j < 0 or j >= H or dfs_board[i][j] != blanco) return;
    dfs_board[i][j] = gris;
    componente.push_back({i,j});
    for(auto &c: vecindad){
        dfs_visit(board,i+c.first, j+c.second, componente);
    }
    dfs_board[i][j] = negro;
}

void dfs(vector<vector<bool>> &board, vector<vector<vector<bool>>> &componentes){
    dfs_board.assign(W, vector<color>(H, blanco));
    forn(i, W) forn(j, H) if(!board[i][j]) dfs_board[i][j] = negro;

    forn(i, W) forn(j, H) if(dfs_board[i][j] == blanco){
        list<ii> componente;
        dfs_visit(board, i,j, componente);
        int max_x =-1, max_y=-1;
        int min_x =W+1, min_y=H+1;
        for(auto p : componente){
            max_x = max(max_x, p.first);
            min_x = min(min_x, p.first);
            max_y = max(max_y, p.second);
            min_y = min(min_y, p.second);
        }
        vector<vector<bool>> m(max_x-min_x+1, vector<bool>(max_y-min_y+1, false));
        for(auto p : componente){
            m[p.first-min_x][p.second-min_y] = true;
        }
        componentes.push_back(m);
    }
}


void rotar(vector<vector<bool>> &m1){
    vector<vector<bool>> res(m1[0].size(), vector<bool>(m1.size(), false));
    forn(i, m1.size()) forn(j, m1[0].size()) res[j][m1.size()-i-1] = m1[i][j];
    m1 = res;

}

bool isomorfos(vector<vector<bool>> &m1, vector<vector<bool>> &m2){
    if (
        !(m1.size() == m2.size() and m1[0].size() == m2[0].size()) and
        !(m1.size() == m2[0].size() and m1[0].size() == m2.size())
    ) return false;
    if(m1 == m2) return true;
    rotar(m1); if(m1 == m2) return true;
    rotar(m1); if(m1 == m2) return true;
    rotar(m1); if(m1 == m2) return true;

    reverse(m1.begin(),m1.end());
    if(m1 == m2) return true;
    rotar(m1); if(m1 == m2) return true;
    rotar(m1); if(m1 == m2) return true;
    rotar(m1); if(m1 == m2) return true;
    return false;
}



void solve(){
    vector<vector<vector<bool>>> componentes1, componentes2;

    dfs(board1, componentes1);
    dfs(board2, componentes2);
    if(componentes1.size() != componentes2.size()){
        cout << "NO\n";
        return;
    }

    for(auto &m1 : componentes1){
        bool found = false;
        for(auto it = componentes2.begin(); it != componentes2.end(); it++){
            auto m2 = *it;
            if(isomorfos(m1,m2)){
                found = true;
                componentes2.erase(it);
                break;
            }
        }
        if(!found) {
            cout << "NO\n"; return;
        }
    }
    cout << "YES\n";
}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    cin >> t;

    forn(i,t){
        cin >> W >> H >> n;
        int x,y;
        board1.assign(W, vector<bool>(H, false));
        board2.assign(W, vector<bool>(H, false));
        forn(j,n){
            cin >> x >> y;
            board1[x][y] = true;
        }
        forn(j,n){
            cin >> x >> y;
            board2[x][y] = true;
        }
        solve();
    }

    return 0;
}