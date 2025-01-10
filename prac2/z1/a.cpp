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
const int MAXN = 17;
int dp[MAXN][MAXN];
int pref[MAXN];
inline int sum(int l, int r) {
    return pref[r] - (l == 0 ? 0 : pref[l - 1]);
}
int opt_dist(vector<int> a, vector<int> sums) {
    int k = sums.size(), n = a.size();
    // dp[i][j] - najlepszy wynik do i-tego miejsca po ogarnieciu j blokow jedynek
    // dp[i][j] = min(dp[i - 1][j] + (a[i] == '1'), dp[i - #1[j] - 1][j-1] + (a[i - #1[j]] == '1') + #1[j] - sum(i - #1[j], i) )
    for(int i = 0; i < n; i++)
        for(int j = 0; j <= k; j++) 
            dp[i][j] = INF;
    
    pref[0] = a[0];
    for(int i = 1; i < n; i++) {
        pref[i] = a[i] + pref[i - 1];
    }

    dp[0][0] = a[0] == 1;
    if(sums[0] == 1)
        dp[0][1] = a[0] == 0;
    
    for(int i = 1; i < n; i++) {
        for(int j = 1; j <= k; j++) {
            dp[i][j] = dp[i - 1][j] + (a[i] == 1);
            int pos = INF, cnt1 = sums[j - 1];
            if(j == 1 && i - cnt1 + 1 >= 0) {
                pos = (i - cnt1 - 1 >= 0 ? dp[i - cnt1 - 1][j - 1]: 0) + (i - cnt1 >= 0 ? (a[i - cnt1] == 1) : 0) + cnt1 - sum(i - cnt1 + 1, i);
            }
            if(j > 1 && i - cnt1 - 1 >= 0 && dp[i - cnt1 - 1][j - 1] != INF) {
                pos = dp[i - cnt1 - 1][j - 1] + (a[i - cnt1] == 1) + cnt1 - sum(i - cnt1 + 1, i);
            }
            dp[i][j] = min(dp[i][j], pos);
        }
        dp[i][0] = dp[i - 1][0] + (a[i] == 1);
    }
    return dp[n - 1][k];
}
int rand(int a, int b) {
    return a + rand() % (b - a + 1);
}
void generate_random(int n, int m, vvi &tab, vvi &tab_rev, vi &sum_row, vi &sum_col) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(rand(1, n * m) <= sum_row[i] * sum_col[j]) {
                tab[i][j] = 1;
                tab_rev[j][i] = 1;
            } else {
                tab[i][j] = 0;
                tab_rev[j][i] = 0;
            }
        }
    }
}
void solve(int n, int m, vvi rows, vvi cols) {
    vvi tab(n, vi(m, 0));
    vvi tab_rev(m, vi(n, 0));
    vi sum_row(n, 0), sum_col(m, 0);
    for(int i = 0; i < n; i++) 
        for(auto x : rows[i]) 
            sum_row[i] += x;
    for(int j = 0; j < m; j++) 
        for(auto x : cols[j]) 
            sum_col[j] += x;        
    generate_random(n, m, tab, tab_rev, sum_row, sum_col);


    int cnt = 0;
    auto flip = [&](int i, int j) {
        tab[i][j] = tab[i][j] ^ 1;
        tab_rev[j][i] = tab_rev[j][i] ^ 1;
    };

    int wrong[2 * MAXN];
    memset(wrong, 0, sizeof(wrong));

    while(true) {

        int cnt_wrong = 0;
        for(int i = 0; i < n; i++) {
            if(opt_dist(tab[i], rows[i]) > 0) wrong[cnt_wrong++] = i;
        }
        for(int j = 0; j < m; j++) {
            if(opt_dist(tab_rev[j], cols[j]) > 0) wrong[cnt_wrong++] = n + j;
        }
        if(cnt_wrong == 0) 
            break;
        
        int x = rand(0, cnt_wrong);

        if(wrong[x] < n) {
            x = wrong[x];
            if(rand(1,100) <= 20) {
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
            flip(x,best_opt_j);
        } else {
            x = wrong[x] - n;
            if(rand(1,100) <= 20) {
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
            flip(best_opt_i,x);
        }

        if(cnt++ % 10000 == 0) {
            generate_random(n, m, tab, tab_rev, sum_row, sum_col);
        }
    }
    cout << cnt % 10000 << "\n";
    ofstream out("zad_output.txt");
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
vi parse_line(string line) {
    int cur = 0;
    vi res;
    for(int i = 0; i < (int)line.length(); i++) {
        if(line[i] == ' ') {
            res.pb(cur), cur = 0;
        } else {
            cur *= 10, cur += line[i] - '0';
        }
    }
    res.pb(cur);
    return res;
}
int32_t main() {
    srand(time(nullptr));

    ifstream file("zad_input.txt");
    int n, m;
    file >> n >> m;
    vvi rows(n), cols(m);
    string line;
    getline(file, line);
    for(int i = 0; i < n; i++) {
        getline(file, line);
        rows[i] = parse_line(line);
    }
    for(int i = 0; i < m; i++) {
        getline(file, line);
        cols[i] = parse_line(line);
    }
    solve(n, m, rows, cols);
    
    file.close();

    return 0;
}