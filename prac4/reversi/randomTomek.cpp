#include <vector>
#include <iostream>
#include <random>

using namespace std;

const int INF = 1e9;
const int BLACK = 0;
const int WHITE = 1;
int ME;
int OPP;

struct GState {
    int turn; // 0 - czarny, 1 - bialy
    vector<vector<int>> board; // -1 -> puste, 0 -> czarny, 1 -> bialy
};

vector<vector<int>> initial_board() {
    vector<vector<int>> b(8, vector<int>(8, -1));
    b[3][3] = 1;
    b[4][4] = 1;
    b[3][4] = 0;
    b[4][3] = 0;

    return b;
}

int ox[8] = {-1, -1, -1, 1, 1, 1, 0, 0};
int oy[8] = {-1, 0, 1, -1, 0, 1, -1, 1};

bool in(int x, int y) {
    return x>=0 && x<8 && y>=0 && y<8;
}

vector<pair<int,int>> killed(int x, int y, vector<vector<int>>& b, int player) {
    if (!in(x, y) || b[x][y] != -1) return {};
    vector<pair<int,int>> k;
    for (int r = 0; r < 8; r ++) {
        int curx = x+ox[r], cury = y+oy[r];
        vector<pair<int,int>> cand;
        while (in(curx, cury) && b[curx][cury] == 1-player) {
            cand.push_back({curx, cury});
            curx += ox[r];
            cury += oy[r];
        }
        if (in(curx, cury) && b[curx][cury] == player) {
            for (auto it : cand) k.push_back(it);
        }
    }
    return k;
}

bool possible(int x, int y, vector<vector<int>>& b, int player) {
    if (!in(x, y) || b[x][y] != -1) return false;
    for (int r = 0; r < 8; r ++) {
        int curx = x+ox[r], cury = y+oy[r];
        int cnt = 0;
        while (in(curx, cury) && b[curx][cury] == 1-player) {
            cnt ++;
            curx += ox[r];
            cury += oy[r];
        }
        if (in(curx, cury) && b[curx][cury] == player && cnt > 0) return true; 
    }
    return false;
}

vector<pair<int,int>> generate_actions(GState &current) {
    vector<pair<int,int>> actions;
    for (int x = 0; x < 8; x ++) {
        for (int y = 0; y < 8; y ++) {
            bool czy = possible(x, y, current.board, current.turn);
            if (czy) {
                actions.push_back({x, y});
            }
        }
    }
    if (actions.size() == 0) actions.push_back({-1, -1});
    return actions;
}

GState move(int x, int y, GState &current) {
    if (x==-1) {
        GState cp = current;
        cp.turn ^= 1;
        return cp;
    }

    vector<pair<int,int>> k = killed(x, y, current.board, current.turn);

    GState child = current;
    for (auto p : k) {
        assert(child.board[p.first][p.second] == 1-current.turn);
        child.board[p.first][p.second] = current.turn;
    }
    child.board[x][y] = current.turn;
    child.turn ^= 1;
    
    return child;
}

pair<int,int> random_move(GState& current) {
    auto actions = generate_actions(current);
    return actions[rand() % (int)actions.size()];
}

double player_move_time, player_remaining_time;

clock_t s_time;
int moves_done;

int main() {
    ios_base::sync_with_stdio(false);
    cout.tie(0);

    srand(time(0));
    
    //init_zobrist();

    GState start;
    start.turn = 0;
    start.board = initial_board();

    GState current;

    bool FIRST_MOVE = true, NEW_GAME = true;
    while (true) {

        if (NEW_GAME) {
            moves_done = 0;
            FIRST_MOVE = true;
            current = start;
            NEW_GAME = false;
            cout << "RDY\n";
            //fflush(stdout);
            continue;
        }

        string com;
        cin >> com;

        if (com == "BYE") break;

        if (com == "ONEMORE") {
            NEW_GAME = true;
            continue;
        }

        if (com == "UGO") {
            cin >> player_move_time >> player_remaining_time;

            if (FIRST_MOVE) {
                ME = BLACK;
                OPP = WHITE;
                FIRST_MOVE = false;
            }
        } else if (com == "HEDID") {
            cin >> player_move_time >> player_remaining_time;

            if (FIRST_MOVE) {
                ME = WHITE;
                OPP = BLACK;
                FIRST_MOVE = false;
            }

            int x, y;
            cin >> y >> x;
           // cerr << "on : " << x << ' ' << y << '\n';

            current = move(x, y, current);
            moves_done ++;
        }

        auto res = random_move(current);

        current = move(res.first, res.second, current);
        moves_done ++;
        //cerr << ME << '\n';
        cout << "IDO " << res.second << ' ' << res.first << '\n';
    }

    return 0;
}