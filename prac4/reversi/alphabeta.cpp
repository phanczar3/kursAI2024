#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <chrono>
#include <random>
#include <map>

using namespace std;
typedef long long ll;
typedef long double ld;
int rand(int a, int b) {
    return a + rand() % (b - a + 1);
}
struct State {
    ll hash;
    int cur, pass, on_board;
    int tab[8][8];
    State() {
        cur = 0, pass = 0, on_board = 4;
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                tab[i][j] = 0;
            }
        }
    };
    void print() {
        cout << hash << "\n";
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                cout << tab[i][j];
            }
            cout << "\n";
        }
    }
};
struct Move {
    int row, col;
};
unordered_map<ll, pair<int, Move>> zhashes;
struct ZHash {
    ll hashes[8][8][2];
    ll not_black;
    mt19937_64 rng;
    ZHash() {
        mt19937_64 rng(1);//chrono::steady_clock::now().time_since_epoch().count());
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                hashes[i][j][0] = rint();
                hashes[i][j][1] = rint();
            }
        }
        not_black = rint();
    }
    ll rint() {
        return uniform_int_distribution<ll>(-(1LL << 62), (1LL << 62))(rng);
    }
    void calc_zhash(State &s) {
        ll res = 0;
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                if(s.tab[i][j] == 1) res ^= hashes[i][j][1];
                if(s.tab[i][j] == 2) res ^= hashes[i][j][0];
            }
        }
        if(s.cur == 1) res ^= not_black;
        s.hash = res;
    }
    void zhash_pos(State &s, int i, int j, int x) {
        if(s.tab[i][j] == 1) s.hash ^= hashes[i][j][1];
        if(s.tab[i][j] == 2) s.hash ^= hashes[i][j][0];
        if(x == 1) s.hash ^= hashes[i][j][1];
        if(x == 2) s.hash ^= hashes[i][j][0];
    }
    void zhash_turn(State &s) {
        s.hash ^= not_black;
    }
} zhash;
State starting_state() {
    State s;
    s.cur = 2;
    s.tab[3][3] = 1, s.tab[4][4] = 1, s.tab[3][4] = 2, s.tab[4][3] = 2;
    s.pass = 0;
    zhash.calc_zhash(s);
    return s;
}
vector<pair<int,int>> directions{{0,1},{1,0},{0,-1},{-1,0},{1,1},{1,-1},{-1,1},{-1,-1}};
bool is_possible(int row, int col, State &s) {
    int opponent = 3 ^ s.cur;
    for(auto p : directions) {
        int nrow = row + p.first, ncol = col + p.second;
        while(nrow < 8 && nrow >= 0 && ncol < 8 && ncol >= 0 && s.tab[nrow][ncol] == opponent) {
            nrow += p.first, ncol += p.second;
        }
        if(nrow == row + p.first && ncol == col + p.second)
            continue;
        if(nrow < 8 && nrow >= 0 && ncol < 8 && ncol >= 0 && s.tab[nrow][ncol] == s.cur) {
            return true;
        }
    }
    return false;
}
vector<Move> generate_possible(State &s) {
    vector<Move> res;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(s.tab[i][j] != 0) 
                continue;
            if(is_possible(i, j, s)) {
                res.push_back({i, j});
            }
        }
    }   
    if((int)res.size() == 0) {
        res.push_back({-1, -1});
    }
    return res;
}
void change_state(State &s, Move &m) {
    if(m.row == -1) {
        s.cur ^= 3;
        s.pass++;
        zhash.zhash_turn(s);
        return;
    }
    int opponent = 3 ^ s.cur;
    for(auto p : directions) {
        int nrow = m.row + p.first, ncol = m.col + p.second;
        while(nrow < 8 && nrow >= 0 && ncol < 8 && ncol >= 0 && s.tab[nrow][ncol] == opponent) {
            nrow += p.first, ncol += p.second;
        }
        if(nrow == m.row + p.first && ncol == m.col + p.second) 
            continue;
        bool to_change = false;
        if(nrow < 8 && nrow >= 0 && ncol < 8 && ncol >= 0 && s.tab[nrow][ncol] == s.cur) {
            to_change = true;
        }
        if(!to_change)
            continue;
        nrow = m.row + p.first, ncol = m.col + p.second;
        while(nrow < 8 && nrow >= 0 && ncol < 8 && ncol >= 0 && s.tab[nrow][ncol] == opponent) {
            zhash.zhash_pos(s, nrow, ncol, s.cur);
            s.tab[nrow][ncol] = s.cur;
            nrow += p.first, ncol += p.second;
        }
    }
    zhash.zhash_pos(s, m.row, m.col, s.cur);
    s.tab[m.row][m.col] = s.cur;
    s.cur ^= 3;
    s.pass = 0;
    zhash.zhash_turn(s);
    s.on_board++;
}
// is_final returns 1/0/-1/2 if our_player won/tied/lost/not_resolved
int final(State &s, int our_player) {
    if(s.pass == 2 || s.on_board == 64) {
        int cnt = 0;
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                cnt += s.tab[i][j] == our_player;
            }
        }
        if(cnt > s.on_board - cnt) {
            return 1;
        } else if(cnt < s.on_board - cnt) {
            return -1;
        } else {
            return 0;
        }
    }
    return 2;
}
const int INF = 1000000000;
const int WIN = 10000;
const int max_depth = 7;
int values[8][8] = {
    { 5, -3,  2,  2,  2,  2, -3,  5},
    {-3, -5, -1, -2, -2, -1, -5, -3},
    { 2, -1,  1,  0,  0,  1, -1,  2},
    { 2, -2,  0,  1,  1,  0, -2,  2},
    { 2, -2,  0,  1,  1,  0, -2,  2},
    { 2, -1,  1,  0,  0,  1, -1,  2},
    {-3, -5, -1, -2, -2, -1, -5, -3},
    { 5, -3,  2,  2,  2,  2, -3,  5} 
};
int evaluate(State &s, int our_player) {
    int result = 0;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            int mul;
            if(s.tab[i][j] == our_player) mul = 1;
            else if(s.tab[i][j] == (3 ^ our_player)) mul = -1;
            else mul = 0;
            result += values[i][j] * mul;
        }
    }
    return result;
}
// 1 - MAX, 0 - MIN
pair<int, Move> alphabeta(State s, int depth, int alpha, int beta, int player, int our_player) {
    auto it = zhashes.find(s.hash); // hash
    if(it != zhashes.end()) {
        return (*it).second; // hash
    }

    int result = final(s, our_player);
    if(result != 2) {
        return {result*WIN,{-2,-2}};
    }
    if(depth == max_depth) {
        return {evaluate(s, our_player), {-2,-2}};
    }
    vector<Move> moves = generate_possible(s);
    if(depth <= 3) {
        vector<pair<Move, State>> options(moves.size());
        for(int i = 0; i < (int)moves.size(); i++) {
            State new_s = s;
            change_state(new_s, moves[i]);
            options[i] = {moves[i], new_s};
        }
        if(player == 1) {
            sort(options.begin(), options.end(), [&](pair<Move, State> &p1, pair<Move, State> &p2) {
                return evaluate(p1.second, our_player) > evaluate(p2.second, our_player);
            });
        } else {
            sort(options.begin(), options.end(), [&](pair<Move, State> &p1, pair<Move, State> &p2) {
                return evaluate(p1.second, our_player) < evaluate(p2.second, our_player);
            });
        }
        Move res;
        int best_score = (player == 1 ? -INF : INF);
        for(int i = 0; i < (int)options.size(); i++) {
            pair<int,Move> p = alphabeta(options[i].second, depth + 1, alpha, beta, 1 - player, our_player);
            if(player == 1 && p.first >= beta) {
                zhashes[s.hash] = {p.first, options[i].first}; // hash
                return {p.first, options[i].first};
            }
            if(player == 0 && p.first <= alpha) {
                zhashes[s.hash] = {p.first, options[i].first}; // hash
                return {p.first, options[i].first};
            }
            if(player == 1) {
                alpha = max(alpha, p.first);
            } else {
                beta = min(beta, p.first);
            }
            if(player == 1 && best_score < p.first) {
                best_score = p.first, res = options[i].first;
            } else if(player == 0 && best_score > p.first) {
                best_score = p.first, res = options[i].first;
            }
        }
        zhashes[s.hash] = {best_score, res}; // hash
        return {best_score, res};
    } else {
        Move res;
        int best_score = (player == 1 ? -INF : INF);
        for(int i = 0; i < (int)moves.size(); i++) {
            State new_s = s;
            change_state(new_s, moves[i]);
            pair<int,Move> p = alphabeta(new_s, depth + 1, alpha, beta, 1 - player, our_player);
            if(player == 1 && p.first >= beta) {
                zhashes[s.hash] = {p.first, moves[i]}; // hash
                return {p.first, moves[i]};
            }
            if(player == 0 && p.first <= alpha) {
                zhashes[s.hash] = {p.first, moves[i]}; // hash
                return {p.first, moves[i]};
            }
            if(player == 1) {
                alpha = max(alpha, p.first);
            } else {
                beta = min(beta, p.first);
            }
            if(player == 1 && best_score < p.first) {
                best_score = p.first, res = moves[i];
            } else if(player == 0 && best_score > p.first) {
                best_score = p.first, res = moves[i];
            }
        }
        zhashes[s.hash] = {best_score, res}; // hash
        return {best_score, res};
    }
}
Move choose_move(State &s) {
    return alphabeta(s, 0, -INF, INF, 1, s.cur).second;
}
void print_move(Move &move) {
    cout << "IDO " << move.col << " " << move.row << "\n";
}
Move read_move() {
    int row, col;
    cin >> col >> row;
    return {row, col};
}
string play_game() {
    State state = starting_state();
    string msg;
    while(true) {
        cin >> msg;
        if(msg == "UGO") {
            ld t1, t2;
            cin >> t1 >> t2;
            vector<Move> moves = generate_possible(state);
            Move cur_move = choose_move(state);
            print_move(cur_move);
            change_state(state, cur_move);
            zhashes.clear();
            // state.print();
        } else if(msg == "HEDID") {
            ld t1, t2;
            cin >> t1 >> t2;
            Move cur_move = read_move();
            change_state(state, cur_move);
            vector<Move> moves = generate_possible(state);
            Move my_move = choose_move(state);
            print_move(my_move);
            change_state(state, my_move);
            zhashes.clear();
            // state.print();
        } else {
            return msg;
        }
    }
}
int main() {
    srand(time(nullptr));

    string msg;
    while(true) {
        cout << "RDY\n";
        msg = play_game();
        if(msg == "BYE") {
            break;
        }
    }


    return 0;
}