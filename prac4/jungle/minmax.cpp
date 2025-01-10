#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;
typedef long double ld;
int rand(int a, int b) {
    return a + rand() % (b - a + 1);
}
struct Piece {
    int id, typ;
    int row, col;
    // szczur (R), kot (C), pies (D), wilk (W), 
    // pantera (J), tygrys (T), lew (L), slon (E)
};
struct Move {
    int piece_id, row1, col1, row2, col2;
};
struct Player {
    vector<Piece> pieces;
};
struct State {
    vector<Player> p;
    int cur;
    int tab[9][7];
    State() {
        for(int i = 0; i < 9; i++) {
            for(int j = 0; j < 7; j++) {
                tab[i][j] = 0;
            }
        }
    }
    void print() {
        cout << cur << "\n";
        for(int i = 0; i < 9; i++) {
            for(int j = 0; j < 7; j++) {
                cout << tab[i][j];
            }
            cout << "\n";
        }
    }
};
State starting_state() {
    Player p1, p2;
    p1.pieces =  
        {{1, 1, 6, 6},
        {2, 2, 7, 1},
        {3, 3, 7, 5},
        {4, 4, 6, 2},
        {5, 5, 6, 4},
        {6, 6, 8, 0},
        {7, 7, 8, 6},
        {8, 8, 6, 0}};
    p2.pieces = 
        {{9, 1, 2, 0},
        {10, 2, 1, 5},
        {11, 3, 1, 1},
        {12, 4, 2, 4},
        {13, 5, 2, 2},
        {14, 6, 0, 6},
        {15, 7, 0, 0},
        {16, 8, 2, 6}};
    State res;
    res.p = {p1, p2};
    for(int i = 0; i < 8; i++) {
        int &row = p1.pieces[i].row, &col = p1.pieces[i].col;
        res.tab[row][col] = p1.pieces[i].id;
    }
    for(int i = 0; i < 8; i++) {
        int &row = p2.pieces[i].row, &col = p2.pieces[i].col;
        res.tab[row][col] = p2.pieces[i].id;
    }
    res.cur = 0;
    return res;
}
vector<pair<int,int>> directions{{1,0},{0,1},{-1,0},{0,-1}};
inline bool is_pond(int row, int col) {
    return 
        ((1 <= col && col <= 2) || (4 <= col && col <= 5)) && 
        (3 <= row && row <= 5);
}
inline bool same_player(int id1, int id2) {
    return !(id1 <= 8 ^ id2 <= 8);
}
// is den of this player
inline bool own_den(int id, int row, int col) {
    return 
        (row == 8 && col == 3 && id <= 8) || 
        (row == 0 && col == 3 && id > 8);
} 
// can typ1 capture typ2
inline bool can_capture(int typ1, int typ2) {
    if(typ1 != 1 && typ1 != 8) {
        return typ1 >= typ2;
    } else if(typ1 == 1) {
        return typ2 == 1 || typ2 == 8;
    } else {
        return typ2 > 1;
    }
}
inline bool is_traps(int row, int col) {
    return 
        (row == 0 && (col == 2 || col == 4)) ||
        (row == 1 && col == 3) ||
        (row == 8 && (col == 2 || col == 4)) ||
        (row == 7 && col == 3);
}
inline bool rat_blocking1(int row, int col, int drow, int tab[9][7]) {
    return 
        (tab[row][col] != 0) ||
        (tab[row + drow][col] != 0) ||
        (tab[row + 2 * drow][col] != 0);
}
inline bool rat_blocking2(int row, int col, int dcol, int tab[9][7]) {
    return
        (tab[row][col] != 0) ||
        (tab[row][col + dcol] != 0);
}
vector<Move> generate_possible(State &s) {
    Player player = s.p[s.cur];
    vector<Move> res;

    for(auto& piece : player.pieces) {
        int row = piece.row, col = piece.col;
        for(auto& new_directions : directions) {
            int nrow = row + new_directions.first, ncol = col + new_directions.second;
            // out of board
            if(nrow < 0 || nrow > 8 || ncol < 0 || ncol > 6) {
                continue;
            }
            // my own den
            if(own_den(piece.id, nrow, ncol)) {
                continue;
            }
            // lion and tiger jump
            if((piece.typ == 6 || piece.typ == 7) && is_pond(nrow, ncol)) {
                if(new_directions.first != 0 && !rat_blocking1(nrow, ncol, new_directions.first, s.tab)) {
                    nrow += 3 * new_directions.first;
                } else if(new_directions.second != 0 && !rat_blocking2(nrow, ncol, new_directions.second, s.tab)) {
                    ncol += 2 * new_directions.second;
                }
            }
            // space occupied
            if(s.tab[nrow][ncol] != 0 && same_player(piece.id, s.tab[nrow][ncol])) {
                continue;
            }
            // only rat in pond
            if(piece.typ != 1 && is_pond(nrow, ncol)) {
                continue;
            }
            if(s.tab[nrow][ncol] == 0) {
                // not occupied
                res.push_back({piece.id, row, col, nrow, ncol});
            } else {
                // from water to land
                int piece2typ = (s.tab[nrow][ncol] % 8 == 0 ? 8 : s.tab[nrow][ncol] % 8);
                if(is_pond(row, col)  && !is_pond(nrow, ncol))
                    continue;
                if(can_capture(piece.typ, piece2typ) || // higher rank 
                (is_traps(nrow, ncol) && (piece.typ != 8 || piece2typ != 1)) // traps
                ) {
                    res.push_back({piece.id, row, col, nrow, ncol});
                }
                
            }
        }
    }
    return res;
}
// changing state s given move m
void change_state(State &s, Move m) {
    Player &p1 = s.p[s.cur], &p2 = s.p[s.cur ^ 1];
    int cnt = 0;
    while(cnt < (int)p1.pieces.size() && p1.pieces[cnt].id != m.piece_id) cnt++;
    assert(cnt < (int)p1.pieces.size());
    if(s.tab[m.row2][m.col2] != 0) {
        int cnt2 = 0;
        while(cnt2 < (int)p2.pieces.size() && p2.pieces[cnt2].id != s.tab[m.row2][m.col2]) cnt2++;
        assert(cnt2 < (int)p2.pieces.size());
        p2.pieces.erase(p2.pieces.begin() + cnt2);
    }
    s.tab[m.row1][m.col1] = 0;
    s.tab[m.row2][m.col2] = m.piece_id;
    p1.pieces[cnt].row = m.row2, p1.pieces[cnt].col = m.col2;
    s.cur ^= 1;
}
// szczur (R), kot (C), pies (D), wilk (W), 
// pantera (J), tygrys (T), lew (L), slon (E)

int values[2][9][7] =
{
    {
        {11, 12, 40, 200, 40, 12, 11},
        {10, 11, 12,  40, 12, 11, 10},
        { 9, 10, 11,  12, 11, 10,  9},
        { 8,  9, 10,  11, 10,  9,  8},
        { 7,  8,  9,  10,  9,  8,  7},
        { 6,  7,  8,   9,  8,  7,  6},
        { 5,  6,  7,   8,  7,  6,  5},
        { 4,  5,  6,   7,  6,  5,  4},
        { 3,  4,  5,   6,  5,  4,  3},
    },
    {
        { 3,  4,  5,   6,  5,  4,  3},
        { 4,  5,  6,   7,  6,  5,  4},
        { 5,  6,  7,   8,  7,  6,  5},
        { 6,  7,  8,   9,  8,  7,  6},
        { 7,  8,  9,  10,  9,  8,  7},
        { 8,  9, 10,  11, 10,  9,  8},
        { 9, 10, 11,  12, 11, 10,  9},
        {10, 11, 12,  40, 12, 11, 10},
        {11, 12, 40, 200, 40, 12, 11},
    }
};
// evaluate state for player
int evaluate(const State &state, int player) {
    int mul = (player == 0 ? 1 : -1); 
    
    Player p1 = state.p[0], p2 = state.p[1];
    int res = 0;
    int idx = 0;
    for(int i = 1; i <= 8; i++) {
        while(idx < (int)p1.pieces.size() && p1.pieces[idx].id < i) idx++;
        if(idx < (int)p1.pieces.size() && p1.pieces[idx].id == i) {
            res += values[0][p1.pieces[idx].row][p1.pieces[idx].col] * mul;
        } else {
            res += -100 * mul;
        }
    }
    idx = 0;
    for(int i = 9; i <= 16; i++) {
        while(idx < (int)p2.pieces.size() && p2.pieces[idx].id < i) idx++;
        if(idx < (int)p2.pieces.size() && p2.pieces[idx].id == i) {
            res -= values[1][p2.pieces[idx].row][p2.pieces[idx].col] * mul;
        } else {
            res -= -100 * mul;
        }
    }
    return res;
}
const int INF = 1e9;
const int max_depth = 4;
// 1 - MAX, 0 - MIN
pair<int, Move> min_max(State &state, int depth, int player, int our_player) {
    if(depth == max_depth) {
        return {evaluate(state, our_player), {}};
    }
    vector<Move> moves = generate_possible(state);
    Move res;
    int best_score = (player == 1 ? -INF : INF);
    for(int i = 0; i < (int)moves.size(); i++) {
        State s = state;
        change_state(s, moves[i]);
        pair<int, Move> p = min_max(s, depth + 1, 1 - player, our_player);
        if(player == 1) {
            if(best_score < p.first) {
                best_score = p.first;
                res = moves[i];
            }
        } else {
            if(best_score > p.first) {
                best_score = p.first;
                res = moves[i];
            }
        }
    }  
    return {best_score, res};
}
Move choose_move(State &state) {
    return min_max(state, 0, 1, state.cur).second;
}
void print_move(const Move &m) {
    cout << "IDO " << m.col1 << " " << m.row1 << " " << m.col2 << " " << m.row2 << "\n";
}
Move read_move(const State &s) {
    int row1, col1, row2, col2;
    cin >> col1 >> row1 >> col2 >> row2;
    return {s.tab[row1][col1], row1, col1, row2, col2};
}
string play_game() {
    State state = starting_state();
    string msg;
    while(true) {
        cin >> msg;
        if(msg == "UGO") {
            ld t1, t2;
            cin >> t1 >> t2;
            Move cur_move = choose_move(state);
            print_move(cur_move);
            change_state(state, cur_move);
        } else if(msg == "HEDID") {
            ld t1, t2;
            cin >> t1 >> t2;
            Move cur_move = read_move(state);
            change_state(state, cur_move);
            Move my_move = choose_move(state);
            print_move(my_move);
            change_state(state, my_move);
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