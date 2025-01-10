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
vii moves{{1,0},{-1,0},{0,1},{0,-1}};
vector<char> moves2{'D','U','R','L'};
const int INF = 1000;
vvi dist_manhattan;
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
set<ii> get_start(vector<vector<char>> &tab) {
    int n = tab.size(), m = tab[0].size();
    set<ii> start;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(tab[i][j] == 'S' || tab[i][j] == 'B') {
                start.insert({i,j});
            }
        }
    }
    return start;
}
void update_dist_manhattan(vector<vector<char>> &tab) {
    int n = tab.size(), m = tab[0].size();
    dist_manhattan.assign(n, vi(m, INF));

    queue<ii> q;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(tab[i][j] == 'B' || tab[i][j] == 'G') {
                dist_manhattan[i][j] = 0;
                q.push({i,j});
            }
        }
    }
    while(q.size() > 0) { 
        ii u = q.front();
        q.pop();

        for(int i = 0; i < 4; i++) {
            ii diff = moves[i];
            if(u.fi + diff.fi < 0 || u.se + diff.se < 0 || u.fi + diff.fi >= n || u.se + diff.se >= m
            || tab[u.fi + diff.fi][u.se + diff.se] == '#') continue;
            if(dist_manhattan[u.fi + diff.fi][u.se + diff.se] == INF) {
                dist_manhattan[u.fi + diff.fi][u.se + diff.se] = dist_manhattan[u.fi][u.se] + 1;
                q.push({u.fi + diff.fi, u.se + diff.se});
            }
        }
    }
}
int h(set<ii> &a) {
    int res = 0;
    for(auto p : a) {
        res = max(res, dist_manhattan[p.fi][p.se]);
    }
    return res;
}
map<set<ii>, int> dist;
int g(set<ii> &a) {
    auto it = dist.find(a);
    if(it == dist.end()) {
        dist[a] = INF;
        return INF;
    }
    return (*it).se;
}
int f(set<ii> &a) {
    return h(a) + g(a);
}
class Compare {
    public:
        bool operator()(pair<set<ii>,int> &a, pair<set<ii>,int> &b) {
            return f(a.fi) > f(b.fi);
        }
};
string astar(vector<vector<char>> &tab, set<ii> &start) {
    int n = tab.size(), m = tab[0].size();
    dist.clear();

    map<set<ii>, string> result;
    priority_queue<pair<set<ii>,int>, vector<pair<set<ii>,int>>, Compare> q;
    set<ii> ans;
    dist[start] = 0;
    result[start] = "";
    q.push({start,0});

    while(q.size() > 0) {
        pair<set<ii>,int> cur = q.top();
        q.pop();
        if(cur.se != g(cur.fi)) {
            continue;
        }
        set<ii> u;
        swap(u, cur.fi);
        if(h(u) == 0) {
            return result[u];
        }

        for(int j = 0; j < 4; j++) {
            ii diff = moves[j];
            set<ii> v;
            for(auto p : u) {
                int new_r = diff.fi + p.fi, new_c = diff.se + p.se;
                if(new_r < 0 || new_r >= n || new_c < 0 || new_c >= m || (tab[new_r][new_c] == '#')) {
                    v.insert(p);
                } else {
                    v.insert({new_r, new_c});
                }
            }
            if(g(u) + 1 < g(v)) {
                dist[v] = g(u) + 1;
                q.push({v, g(v)});
                // prev[v] = {u, moves2[j]};
                result[v] = result[u] + moves2[j];
            }
        }
    }

    // string result = "";
    // while(ans != start) {
    //     result += prev[ans].se;
    //     ans = prev[ans].fi;
    // }
    // reverse(all(result));
    // return result;
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

    set<ii> start = get_start(tab);
    update_dist_manhattan(tab);
    string ans = astar(tab, start);

    print_result(ans);

    return 0;
}