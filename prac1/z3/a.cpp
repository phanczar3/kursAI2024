/*
    Komentarz do zadania 3
    Piotr Hanczar
    zaimplementowanie zasad gry pokera
    nie trzeba było porownywac kart miedzy soba bo zawsze figurant ma wszystkie karty wieksze od blotkarza
    poniewaz figurant nie moze dobrac koloru lub pokera to moze bylo tego nie sprawdzac, ja i tak to sprawdzam

    najliczniejszym układem jaki udało mi sie znaleźć dającym 50% jest 8,9,10 we wszystkich kolorach (12 kart)
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
struct Card {
    // v - 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
    // c - 0, 1, 2, 3
    int v, c;
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
bool is_straight(vector<Card> hand) {
    bool is_step = true;
    for(int i = 0; i < 4; i++) {
        is_step &= hand[i].v + 1 == hand[i+1].v;
    }
    return is_step;
}
bool is_poker(vector<Card> hand) {
    return is_flush(hand) && is_straight(hand);
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
int rand(int a, int b) {
    return a + rand() % (b - a + 1);
}
void my_shuffle(vector<Card> &hand) {
    for(int i = 0; i < (int)hand.size(); i++) {
        int j = rand(0, i);
        swap(hand[i], hand[j]);
    }
}
void test(int no_of_tests) {
    for(int rep = 0; rep < no_of_tests; rep++) {
        my_shuffle(fig);
        my_shuffle(blot);
        vector<Card> cur_fig(fig.begin(), fig.begin() + 5);
        vector<Card> cur_blot(blot.begin(), blot.begin() + 5);
        auto cmp = [&](Card &a, Card &b) {
            if(a.v == b.v) {
                return a.c < b.c;
            } else return a.v < b.v;
        };
        sort(all(cur_fig), cmp);
        sort(all(cur_blot), cmp);
        if(is_win(cur_blot, cur_fig)) wins++;
    }
}
int32_t main() {

    srand(time(nullptr));

    for(int v = 2; v <= 10; v++) {
        for(int c = 0; c <= 3; c++) {
            blot.pb({v,c});
        }
    }
    // for(int v = 8; v <= 10; v++) {
    //     for(int c = 0; c <= 3; c++) {
    //         blot.pb({v, c});
    //     }
    // }
    for(int v = 11; v <= 14; v++) {
        for(int c = 0; c <= 3; c++) {
            fig.pb({v, c});
        }
    }
    const int N = 1000000;
    test(N);
    cout << wins << " " << N << " " << fixed << setprecision(4) << (ld)wins / N * 100 << "\n";

    return 0;
}