/*
    Komentarz do zadania 5
    Piotr Hanczar
*/
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <random>
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
int opt_dist(vector<bool> s, int d) {
    int n = s.size();
    int all_sum = 0;
    for(bool x : s) all_sum += x;
    if(d == 0) {
        return all_sum;
    }
    int cur_sum = 0;
    for(int i = 0; i < d; i++) {
        cur_sum += s[i];
        all_sum -= s[i];
    }
    int ans = d - cur_sum + all_sum;
    for(int i = d; i < n; i++) {
        cur_sum += s[i];
        all_sum -= s[i];
        cur_sum -= s[i-d];
        all_sum += s[i-d];
        ans = min(ans, d - cur_sum + all_sum);
    }
    return ans;
}
int rand(int a, int b) {
    return a + rand() % (b - a + 1);
}
void generate_random(int n, int m, vector<vector<bool>> &tab, vector<vector<bool>> &tab_rev) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(rand(1, 100) <= 50) {
                tab[i][j] = 0;
                tab_rev[j][i] = 0;
            } else {
                tab[i][j] = 1;
                tab_rev[j][i] = 1;
            }
        }
    }
}
void solve(int n, int m, vi rows, vi cols) {
    vector<vector<bool>> tab(n, vector<bool>(m, 0));
    vector<vector<bool>> tab_rev(m, vector<bool>(n, 0));
    generate_random(n, m, tab, tab_rev);
    int cnt = 0;
    auto flip = [&](int i, int j) {
        tab[i][j] = tab[i][j] ^ 1;
        tab_rev[j][i] = tab_rev[j][i] ^ 1;
    };
    while(true) {
        vi wrong_rows, wrong_cols;
        for(int i = 0; i < n; i++) {
            if(opt_dist(tab[i], rows[i]) > 0) wrong_rows.pb(i);
        }
        for(int j = 0; j < m; j++) {
            if(opt_dist(tab_rev[j], cols[j]) > 0) wrong_cols.pb(j);
        }
        if(wrong_rows.size() + wrong_cols.size() == 0) break;
        int x = rand(0, (int)wrong_rows.size() + (int)wrong_cols.size() - 1);


        if(x < (int)wrong_rows.size()) {
            if(rand(1,100) <= 2) {
                int y = rand(0, m-1);
                flip(x,y);
                continue;
            }
            int best_opt_j = -1, best_opt = INF;
            for(int j = 0; j < m; j++) {
                flip(x,j);
                int new_row = opt_dist(tab[x], rows[x]), new_col = opt_dist(tab_rev[j], cols[j]);
                if(best_opt > new_row + new_col) {
                    best_opt = new_row + new_col;
                    best_opt_j = j;
                }
                flip(x,j);
            }
            assert(best_opt_j != -1);
            flip(x,best_opt_j);
        } else {
            x -= wrong_rows.size();
            if(rand(1,100) <= 2) {
                int y = rand(0, n-1);
                flip(y,x);
                continue;
            }
            int best_opt_i = -1, best_opt = INF;
            for(int i = 0; i < n; i++) {
                flip(i,x);
                int new_row = opt_dist(tab[i], rows[i]), new_col = opt_dist(tab_rev[x], cols[x]);
                if(best_opt > new_row + new_col) {
                    best_opt = new_row + new_col;
                    best_opt_i = i;
                }
                flip(i,x);
            }
            assert(best_opt_i != -1);
            flip(best_opt_i,x);
        }
        if(cnt++ % 10000 == 0) {
            generate_random(n, m, tab, tab_rev);
        }
    }
    ofstream out("zad5_output.txt");
    // cout << cnt << "\n";
    string ans = "";
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            ans += (tab[i][j] == 0 ? '.':'#');
        }
        ans += "\n";
    }
    out << ans;
    out.close();
}
int32_t main() {
    srand(time(nullptr));
    
    ifstream file("zad5_input.txt");
    int n, m;
    file >> n >> m;
    vi rows(n), cols(m);
    for(int i = 0; i < n; i++) file >> rows[i];
    for(int i = 0; i < m; i++) file >> cols[i];
    solve(n, m, rows, cols);
    
    file.close();

    return 0;
}