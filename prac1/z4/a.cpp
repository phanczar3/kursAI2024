/*
    Komentarz do zadania 4
    Piotr Hanczar
    Obserwacja: Jesli chcemy aby blok 1 był na przedziale [i,i+d-1], 
    to bedziemy musieli zgasić wszystkie bity wcześniej i pozniej oraz 
    zapalic bity w srodku przedzialu
    stąd koszt([i,i+d-1]) = d - suma(i, i+d-1) + suma(0, i-1) + suma(i+d, n-1)
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
int opt_dist(string s, int d) {
    int n = s.length();
    if(d > n) return -1;
    vi pref(n, 0);
    for(int i = 0; i < n; i++) {
        pref[i] = (i > 0 ? pref[i-1] : 0) + (s[i] == '1' ? 1 : 0);
    }
    auto sum = [&](int l, int r) {
        return pref[r] - (l == 0 ? 0 : pref[l-1]);
    };
    if(d == 0) {
        return sum(0, n-1);
    }
    int ans = n;
    for(int i = 0; i < n-d; i++) {
        int cur = d - sum(i, i + d - 1) + (i == 0 ? 0 : sum(0, i-1)) + (i + d == n ? 0 : sum(i+d, n-1));
        ans = min(ans, cur);
    }
    return ans;
}
int opt_dist2(string s, int d) {
    int n = s.length();
    int all_sum = 0;
    for(char x : s) all_sum += x - '0';
    if(d == 0) {
        return all_sum;
    }
    int cur_sum = 0;
    for(int i = 0; i < d; i++) {
        cur_sum += s[i] - '0';
        all_sum -= s[i] - '0';
    }
    int ans = d - cur_sum + all_sum;
    for(int i = d; i < n; i++) {
        cur_sum += s[i] - '0';
        all_sum -= s[i] - '0';
        cur_sum -= s[i-d] - '0';
        all_sum += s[i-d] - '0';
        ans = min(ans, d - cur_sum + all_sum);
    }
    return ans;
}
int32_t main() {

    string s = "0010001000";
    for(int i = 5; i >= 0; i--) {
        cout << i << " " << opt_dist(s, i) << " " << opt_dist2(s,i) << "\n";
    }
    // cout << opt_dist(s, 4) << "\n";
    return 0;
}