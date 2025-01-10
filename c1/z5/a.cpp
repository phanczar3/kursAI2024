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
struct Card {
    // v - 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
    // c - 0, 1, 2, 3
    int v, c;
    Card() {
        v = 0, c = 0;
    }
    Card(ii p) {
        v = p.fi, c = p.se;
    }
};
/*
    poker krolewski
    poker
    kareta
    full
    kolor
    strit
    trojka
    2 pary
    para
    wysoka karta
*/
bool is_flush(vector<Card> hand) {
    bool is_same = true;
    for(int i = 0; i < 5; i++) {
        is_same &= hand[i].c == hand[0].c;
    }
    return is_same;
}
int is_cnt(vector<Card> hand, int val) {
    vi cnt(16, 0);
    for(int i = 0; i < 5; i++) {
        cnt[hand[i].v]++;
    }
    int ret = 0;
    for(int i = 0; i < 16; i++) {
        if(cnt[i] == val) {
            ret++;
        }
    }
    return ret;
}
bool is_straight(vector<Card> hand) {
    if(is_cnt(hand, 1) != 5) return false;
    sort(all(hand), [&](Card &a, Card &b) {
        if(a.v == b.v) {
            return a.c < b.c;
        } else return a.v < b.v;
    });
    bool is_step = true;
    for(int i = 0; i < 4; i++) {
        is_step &= hand[i].v + 1 == hand[i+1].v;
    }
    return is_step;
}
bool is_poker(vector<Card> hand) {
    return is_flush(hand) && is_straight(hand);
}
bool is_four(vector<Card> hand) {
    return is_cnt(hand, 4);
}
bool is_full(vector<Card> hand) {
    return is_cnt(hand, 2) && is_cnt(hand, 3);
}
bool is_three(vector<Card> hand) {
    return is_cnt(hand, 3);
}
bool is_two_pair(vector<Card> hand) {
    return is_cnt(hand, 2) == 2;
}
bool is_pair(vector<Card> hand) {
    return is_cnt(hand, 2) == 1;
}
// cards sorted
bool is_win(vector<Card> blot, vector<Card> fig) {
    vector<function<bool(vector<Card>)>> funs{is_poker, is_four, is_full, is_flush, is_straight, is_three, is_two_pair, is_pair};
    for(auto fun : funs) {
        if(fun(fig)) {
            return false;
        } else if(fun(blot)) {
            return true;
        }
    }
    // wysokie karty w obu przypadkach
    return false;
}
vector<Card> blot, fig;
int wins = 0;
// n -> 0 - 36
ii get_card_blot(int n) {
    return {n % 9 + 2, n / 9};
}
ii get_card_fig(int n) {
    return {n % 4 + 11, n / 4};
}
void solve() {
    vi fig_all(9, 0), blot_all(9, 0);
    int i[5];
    memset(i, 0, sizeof(i));
    vector<function<bool(vector<Card>)>> funs{is_poker, is_four, is_full, is_flush, is_straight, is_three, is_two_pair, is_pair};
    vector<Card> hand(5);
    for(i[0] = 0; i[0] < 36; i[0]++) {
        hand[0] = Card(get_card_blot(i[0]));
        for(i[1] = i[0]+1; i[1] < 36; i[1]++) {
            hand[1] = Card(get_card_blot(i[1]));
            for(i[2] = i[1]+1; i[2] < 36; i[2]++) {
                hand[2] = Card(get_card_blot(i[2]));
                for(i[3] = i[2]+1; i[3] < 36; i[3]++) {
                    hand[3] = Card(get_card_blot(i[3]));
                    for(i[4] = i[3]+1; i[4] < 36; i[4]++) {
                        hand[4] = Card(get_card_blot(i[4]));
                        int cnt = 0;
                        while(cnt < (int)funs.size() && !funs[cnt++](hand)) ;
                        blot_all[cnt - (funs[cnt-1](hand) ? 1 : 0)]++;
                    }   
                } 
            }    
        }
    }
    for(i[0] = 0; i[0] < 16; i[0]++) {
        hand[0] = Card(get_card_fig(i[0]));
        for(i[1] = i[0]+1; i[1] < 16; i[1]++) {
            hand[1] = Card(get_card_fig(i[1]));
            for(i[2] = i[1]+1; i[2] < 16; i[2]++) {
                hand[2] = Card(get_card_fig(i[2]));
                for(i[3] = i[2]+1; i[3] < 16; i[3]++) {
                    hand[3] = Card(get_card_fig(i[3]));
                    for(i[4] = i[3]+1; i[4] < 16; i[4]++) {
                        hand[4] = Card(get_card_fig(i[4]));
                        int cnt = 0;
                        while(cnt < (int)funs.size() && !funs[cnt++](hand)) ;
                        fig_all[cnt - (funs[cnt-1](hand) ? 1 : 0)]++;
                    }   
                } 
            }    
        }
    }
    debug(blot_all);
    debug(fig_all);
    ll ans = 0;
    for(int k = 0; k < 9; k++) {
        ll sum = 0;
        for(int j = k+1; j < 9; j++) {
            sum += fig_all[j];
        }
        ans += sum*blot_all[k];
    }
    ll all_pos = 1646701056;
    cout << ans << " " << fixed << setprecision(4) << (ld) 100 * ans / all_pos << "\n";
}
int32_t main() {

    srand(time(nullptr));

    solve();

    return 0;
}