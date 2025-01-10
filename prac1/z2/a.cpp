/*
    Komentarz do zadania 2
    Piotr Hanczar
    Znajdujemy optymalny podzial programowaniem dynamicznym
    dp[i] - najlepszy wynik na prefiksie do i-tego miejsca 
    jesli mamy slowo od i do j to dp[j] = max(dp[j], dp[i-1] + (j - i + 1) ^ 2);
    slowa trzymalem w unordered_secie, 
    moglbym skorzystac z drzewa trie i wtedy sprawdzałbym czy dany wyraz jest słowem w O(1)
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

unordered_set<string> words;
void get_words() {
    ifstream file("words.txt");
    string s;
    while(getline(file, s)) {
        words.insert(s);
    }
    file.close();
}
vector<string> seperate_line(string line) {
    line = "*" + line;
    int n = line.length();
    vii dp(n, {0,0});
    for(int i = 1; i < n; i++) {
        string s = "";
        for(int j = i; j < n; j++) {
            s += line[j];
            if(words.count(s) && dp[i-1].fi + (j - i + 1) * (j - i + 1) > dp[j].fi) {
                dp[j].fi = dp[i-1].fi + (j - i + 1) * (j - i + 1);
                dp[j].se = i-1;
            }
        }
    }
    vector<string> res;
    int prev = dp[n-1].se, cur = n-1;
    while(cur > 0) {
        string s = "";
        for(int i = prev+1; i <= cur; i++) s += line[i];
        res.pb(s);
        cur = prev;
        prev = dp[prev].se;
    }
    reverse(all(res));
    return res;
}
void solve() {
    ifstream file("zad2_input.txt");

    ofstream("zad2_output.txt") << u8"z\u6c34\U0001d10b";
    ofstream out("zad2_output.txt");

    string line;
    string to_out = "";
    while(getline(file, line)) {
        for(string &s : seperate_line(line)) {
            to_out += s + " ";
        }
        to_out += "\n";
    }
    out << to_out;
    file.close();
    out.close();
} 
int32_t main() {
    
    get_words();
    
    // debug(seperate_line("tamatematykapustkinieznosi"));
    
    solve();

    return 0;
}