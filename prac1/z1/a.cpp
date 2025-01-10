/*
    Komentarz do zadania 1
    Piotr Hanczar
    Znajdujemy rozwiązanie algorytmem BFS
    opis stanu:
        stan: 2/8/8/8/8/8/8 
        0 -> 0 - białe, 1 - czarne
        pozycje: (nr wiersza, nr kolumny)
        1,2 -> bialy krol
        3,4 -> biala wieza
        5,6 -> czarny krol
    funkcja nastepnika generuje tylko legalne ruchy (korzystajac z is_under_attack)
    przy braku legalnych ruchow sprawdzamy czy jest to mat czy pat w tym drugim przypadku kontynuujemy BFS
    jesli wieza zostaje zbita to z tego stanu nie przechodzimy do jej kolejnych stanow (na pewno nie bedzie mata)
*/
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
using namespace std;
void __print(int x) {cerr << x;}
void __print(long x) {cerr << x;}
void __print(long long x) {cerr << x;}
void __print(unsigned x) {cerr << x;}
void __print(unsigned long x) {cerr << x;}
void __print(unsigned long long x) {cerr << x;}
void __print(float x) {cerr << x;}
void __print(double x) {cerr << x;}
void __print(long double x) {cerr << x;}
void __print(char x) {cerr << '\'' << x << '\'';}
void __print(const char *x) {cerr << '\"' << x << '\"';}
void __print(const string &x) {cerr << '\"' << x << '\"';}
void __print(bool x) {cerr << (x ? "true" : "false");}
template<typename T, typename V>
void __print(const pair<T, V> &x) {cerr << '{'; __print(x.first); cerr << ','; __print(x.second); cerr << '}';}
template<typename T>
void __print(const T &x) {int f = 0; cerr << '{'; for (auto &i: x) cerr << (f++ ? "," : ""), __print(i); cerr << "}";}
void _print() {cerr << "]\n";}
template <typename T, typename... V>
void _print(T t, V... v) {__print(t); if (sizeof...(v)) cerr << ", "; _print(v...);}
#ifndef ONLINE_JUDGE
#define debug(x...) cerr << "[" << #x << "] = ["; _print(x)
#else
#define debug(x...)
#endif

#define fi first
#define se second
#define all(x) x.begin(), x.end()
#define pb push_back
#define BOOST ios_base::sync_with_stdio(false),cin.tie(NULL),cout.tie(NULL);
typedef long long ll;
typedef long double ld;
typedef pair<int,int> ii;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<ii> vii;
typedef vector<ll> vll;
const int INF = 1e9;
/*
    stan: 2/8/8/8/8/8/8 
    0 -> 0 - białe, 1 - czarne
    pozycje: (nr wiersza, nr kolumny)
    1,2 -> bialy krol
    3,4 -> biala wieza
    5,6 -> czarny krol
*/
int st_to_msk(const vi st) {
    return st[0] | (st[1] << 1) | (st[2] << 4) | (st[3] << 7) | (st[4] << 10) | (st[5] << 13) | (st[6] << 16); 
}
vi msk_to_st(const int msk) {
    return {msk & 1, (msk >> 1) & 7, (msk >> 4) & 7, (msk >> 7) & 7, (msk >> 10) & 7, (msk >> 13) & 7,(msk >> 16) & 7};
}
vii moves{{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
vii moves2{{-1,0},{1,0},{0,-1},{0,1}};
// czy dane pole jest atakowane przez przeciwnika
bool is_under_attack(const vi st, int row, int col) {
    if(st[0] == 1) {
        for(auto p : moves2) {
            int r = st[3] + p.fi, c = st[4] + p.se;
            while(r >= 0 && r < 8 && c >= 0 && c < 8) {
                if(r == row && c == col) {
                    return true;
                }
                if(r == st[1] && c == st[2]) {
                    break;
                }
                r = r + p.fi, c = c + p.se;
            }
        }
    }
    int r = (st[0] == 1 ? st[1] : st[5]), c = (st[0] == 1 ? st[2] : st[6]);
    return abs(r - row) <= 1 && abs(c - col) <= 1;
}
vvi next_moves(const vi st) {
    vvi ans;
    if(st[0] == 0) {
        for(auto p : moves2) {
            int r = st[3] + p.fi, c = st[4] + p.se;
            while(r >= 0 && r < 8 && c >= 0 && c < 8) {
                if((r == st[1] && c == st[2]) || (r == st[5] && c == st[6])) {
                    break;
                }     
                ans.pb({st[0] ^ 1, st[1], st[2], r, c, st[5], st[6]});
                r = r + p.fi, c = c + p.se;
            }
        }
    }
    int r = (st[0] == 0 ? st[1] : st[5]), c = (st[0] == 0 ? st[2] : st[6]);
    for(auto p : moves) {
        if(r + p.fi < 0 || r + p.fi >= 8 || c + p.se < 0 || c + p.se >= 8) {
            continue;
        }
        if(!is_under_attack(st, r+p.fi, c+p.se)) {
            if(st[0] == 0) {
                ans.pb({1, r+p.fi, c+p.se, st[3], st[4], st[5], st[6]});
            } else {
                ans.pb({0, st[1], st[2], st[3], st[4], r+p.fi, c+p.se});
            }
        }
    }
    return ans;
}
// minimalna liczba ruchow lub INF -> pat
int bfs(vi start) {
    vi dist(1 << 20, -1);
    dist[st_to_msk(start)] = 0;
    queue<int> q;
    q.push(st_to_msk(start));
    while(q.size() > 0) {
        int cur = q.front();
        vi st = msk_to_st(cur);
        q.pop();
        // zbita wieza -> nie bedzie mata
        if(st[3] == st[5] && st[4] == st[6]) {
            continue;
        }
        vvi nxt_moves = next_moves(st);
        if(nxt_moves.size() == 0) {
            assert(st[0] == 1);
            if(is_under_attack(st, st[5], st[6])) {
                return dist[cur];
            } else {
                // PAT
                continue;
            }
        }
        for(vi nxt : nxt_moves) {
            int cur2 = st_to_msk(nxt);
            if(dist[cur2] != -1) {
                continue;
            }
            dist[cur2] = dist[cur] + 1;
            q.push(cur2);
        }
    }
    return INF;
}
vvi bfs_debug(vi start) {
    vi prev(1 << 20, -1);
    vi dist(1 << 20, -1);
    dist[st_to_msk(start)] = 0;
    queue<int> q;
    q.push(st_to_msk(start));
    while(q.size() > 0) {
        int cur = q.front();
        vi st = msk_to_st(cur);
        q.pop();
        // zbita wieza -> nie bedzie mata
        if(st[3] == st[5] && st[4] == st[6]) {
            continue;
        }
        vvi nxt_moves = next_moves(st);
        if(nxt_moves.size() == 0) {
            assert(st[0] == 1);
            if(is_under_attack(st, st[5], st[6])) {
                vvi ans(dist[cur]+1);
                for(int i = dist[cur], u = cur; i >= 0; i--, u = prev[u]) {
                    ans[i] = msk_to_st(u);
                }
                return ans;
            } else {
                // PAT
                continue;
            }
        }
        for(vi nxt : nxt_moves) {
            int cur2 = st_to_msk(nxt);
            if(dist[cur2] != -1) {
                continue;
            }
            dist[cur2] = dist[cur] + 1;
            prev[cur2] = cur;
            q.push(cur2);
        }
    }
    return {{INF}};
}
int parse_col(char x) {
    return x - 'a';
}
int parse_row(char x) {
    return 7 - (x - '1');
}
vi parse_input(string line) {
    stringstream ss(line);
    string c, p1, p2, p3;
    ss >> c >> p1 >> p2 >> p3;
    vi st(7);
    st[0] = (c == "white" ? 0 : 1);
    st[1] = parse_row(p1[1]);
    st[2] = parse_col(p1[0]);
    st[3] = parse_row(p2[1]);
    st[4] = parse_col(p2[0]);
    st[5] = parse_row(p3[1]);
    st[6] = parse_col(p3[0]);
    return st;
}
string ii_to_pos(int row, int col) {
    string ans;
    ans += (col+'a');
    ans += (8 - row + '0');
    return ans;
}
string st_to_string(vi st) {
    string ans = "";
    ans += (st[0] == 0 ? "white " : "black ");
    ans += ii_to_pos(st[1], st[2]) + " ";
    ans += ii_to_pos(st[3], st[4]) + " ";
    ans += ii_to_pos(st[5], st[6]);
    return ans;
}
int32_t main() {

    // tryb wsadowy 
    if(true) {
        ifstream fs("zad1_input.txt");
        ofstream out("zad1_output.txt");
        string line;
        string to_out = "";
        while(getline(fs, line)) {
            vi st = parse_input(line);
            int ans = bfs(st);
            if(ans == INF) {
                to_out += "inf\n";
            } else {
                to_out += to_string(ans);
                to_out += "\n";
            }
        }
        out << to_out;
        fs.close();
        out.close();
    } 
    // tryb debug
    else {
        string line;
        getline(cin, line);
        vi st = parse_input(line);
        vvi ans = bfs_debug(st);
        if(ans.size() == 1 && ans[0][0] == INF) {
            cout << "pat\n";
        } else {
            for(vi cur : ans) {
                cout << st_to_string(cur) << "\n";
            }
        }
    }

    return 0;
}