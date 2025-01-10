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
int rand(int a, int b) {
    return a + rand() % (b - a + 1);
}
void parse_input(vector<vector<char>> &tab) {
    ifstream file("zad_input.txt");
    string line;
    while(getline(file, line)) {
        vector<char> cur_line(line.length());
        for(int i = 0; i < (int)line.length(); i++) cur_line[i] = line[i];
        tab.pb(cur_line);
    }
    file.close();
}
vii moves{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
vector<char> moves2{'D', 'U', 'R', 'L'};
pair<string, set<ii>> decrease_uncertainty(vector<vector<char>> &tab, int no) {
    int n = tab.size(), m = tab[0].size();
    string result = "";
    set<ii> st;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(tab[i][j] == 'S' || tab[i][j] == 'B') 
                st.insert({i, j});
        }
    }
    function<bool(int)> check_move = [&](int r) {
        ii cur = moves[r];
        set<ii> st2;
        for(auto p : st) {
            int new_r = p.fi + cur.fi, new_c = p.se + cur.se;
            if(new_r < 0 || new_r >= n || new_c < 0 || new_c >= m || (tab[new_r][new_c] == '#')) {
                st2.insert(p);
            } else {
                st2.insert({new_r, new_c});
            }
        }
        bool ok = false;
        if(st2.size() < st.size()) {
            st = st2, result += moves2[r], ok = true;
        }
        return ok;
    };
    int prev_r = 0;
    while((int)st.size() > no) {

        int r = rand(0, 3);
        if((prev_r ^ 1) == r) r ^= 1;
        prev_r = r;

        ii cur = moves[r];
        result += moves2[r];
        set<ii> st2;
        for(auto p : st) {
            int new_r = p.fi + cur.fi, new_c = p.se + cur.se;
            if(new_r < 0 || new_r >= n || new_c < 0 || new_c >= m || (tab[new_r][new_c] == '#')) {
                st2.insert(p);
            } else {
                st2.insert({new_r, new_c});
            }
        }
        st = st2;
    }
    return {result,st};
}
string bfs_raw(vector<vector<char>> &tab, set<ii> start) {
    int n = tab.size(), m = tab[0].size();

    set<set<ii>> vis;
    map<set<ii>, pair<set<ii>,char>> prev;
    queue<set<ii>> q;
    set<ii> ans;

    vis.insert(start);
    q.push(start);
    while(q.size() > 0) {
        set<ii> cur = q.front();
        q.pop();
        for(int i = 0; i < 4; i++) {
            ii diff = moves[i];
            set<ii> new_cur;
            bool ok = true;
            for(auto p : cur) {
                int new_r = p.fi + diff.fi, new_c = p.se + diff.se;
                if(new_r < 0 || new_r >= n || new_c < 0 || new_c >= m || tab[new_r][new_c] == '#') {
                    new_cur.insert(p);
                    if(tab[p.fi][p.se] != 'B' && tab[p.fi][p.se] != 'G') 
                        ok = false;
                } else {
                    new_cur.insert({new_r, new_c});
                    if(tab[new_r][new_c] != 'B' && tab[new_r][new_c] != 'G') 
                        ok = false;
                }
            }
            if(vis.count(new_cur)) continue;
            prev[new_cur] = {cur, moves2[i]};
            if(ok) {
                ans = new_cur;
                break;
            }
            vis.insert(new_cur);
            q.push(new_cur);
        } 
    }
    string result = "";
    while(ans != start) {
        result += prev[ans].se;
        ans = prev[ans].fi;
    }
    reverse(all(result));
    return result;
}
void print_result(string &ans) {
    ofstream out("zad_output.txt");
    out << ans;
    out.close();
}
int32_t main() {
    BOOST;
    srand(time(nullptr));


    vector<vector<char>> tab;
    parse_input(tab);
    
    pair<string, set<ii>> p;
    string ans;
    do{ 
        do {
            p = decrease_uncertainty(tab, 2);
        } while(p.fi.length() > 100);
        string s2 = bfs_raw(tab, p.se);
        ans = p.fi + s2;
    } while(ans.length() > 150);
    
    print_result(ans);

    return 0;
}