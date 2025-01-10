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
    Player &player = s.p[s.cur];
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
// return 1/0/-1 if current player won/not resolved/the other player 
int result(State &s) {
    Player &p1 = s.p[s.cur], &p2 = s.p[s.cur ^ 1];
    if(p2.pieces.size() == 0) return 1; // impossible ?
    if(p1.pieces.size() == 0) return -1; 
    if((*p1.pieces.begin()).id <= 8 && s.tab[8][3] != 0) return -1;
    if((*p1.pieces.begin()).id <= 8 && s.tab[0][3] != 0) return 1;
    if((*p1.pieces.begin()).id > 8 && s.tab[8][3] != 0) return 1;
    if((*p1.pieces.begin()).id > 8 && s.tab[0][3] != 0) return -1;
    return 0; 
}
const int MAXN = 20000;
// return -1/0/1 if starting player won/tied/lost
int simulate(State &s, int &cnt) {
    int starting_player = s.cur;
    int passive_count = 0;
    while(true) {
        vector<Move> moves = generate_possible(s);
        if(moves.size() == 0)
            return 0;
        Move &m = moves[rand(0, (int)moves.size() - 1)];
        if(s.tab[m.row2][m.col2] != 0) passive_count = 0;
        else passive_count++;
        cnt++;
        change_state(s, m);
        int r = result(s);
        if(r != 0) {
            if(s.cur == starting_player) {
                return -1 * r;
            } else {
                return r;
            }
        }
        if(passive_count == 50) return 0;
    }
}
Move choose_move(const State &state, const vector<Move> &moves) {
    Move res = *moves.begin();
    vector<int> score(moves.size(), 0);
    int cnt = 0, best_score = 0;
    for(int i = 0; cnt < MAXN; i = (i + 1) % moves.size()) {
        State s = state;
        change_state(s, moves[i]);
        score[i] += simulate(s, cnt);
        if(score[i] > best_score) {
            best_score = score[i];
            res = moves[i];
        }
    }
    return res;
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
            vector<Move> moves = generate_possible(state);
            Move cur_move = choose_move(state, moves);
            print_move(cur_move);
            change_state(state, cur_move);
        } else if(msg == "HEDID") {
            ld t1, t2;
            cin >> t1 >> t2;
            Move cur_move = read_move(state);
            change_state(state, cur_move);
            vector<Move> moves = generate_possible(state);
            Move my_move = choose_move(state, moves);
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