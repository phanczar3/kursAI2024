#include <iostream>
#include <vector>
#include <utility>
#include <queue>

using namespace std;
int a[70][70];
bool is_possible(int row, int col, int len) {
    if(row + len >= 70 || col + len >= 70) return false;
    bool ok = true;
    for(int i = row; i < row + len; i++) {
        for(int j = col; j < col + len; j++) {
            if(a[i][j] != 0) {
                return false;
            }
        }
    }
    return true;
}
void fill_with(int row, int col, int len) {
    for(int i = row; i < row + len; i++) {
        for(int j = col; j < col + len; j++) {
            a[i][j] = len + 'A' - 1;
        }
    }
}
int main() {
    srand(time(nullptr));
    vector<pair<int,int>> v;
    v.push_back({0,0});
    int ans = 0;
    for(int i = 24; i >= 1; i--) { 
        bool inserted = false;
        // for(int j = 0; j < (int)v.size(); j++) {
        //     swap(v[j], v[j + rand() % (j + 1)]);
        // }
        for(int j = 0; j < (int)v.size() && !inserted; j++) {
            auto &p = v[j];
            if(is_possible(p.first, p.second, i)) {
                fill_with(p.first, p.second, i);
                if(p.first + i < 70) {
                    v.push_back({p.first + i, p.second});
                }
                if(p.second + i < 70) {
                    v.push_back({p.first, p.second + i});
                }
                ans += i * i;
                inserted = true;
            }
        }
    }
    cout << 70 * 70 - ans << "\n";
    for(int i = 0; i < 70; i++) {
        for(int j = 0; j < 70; j++) {
            cout << (a[i][j] == 0 ? '.' : (char)a[i][j]);
        }
        cout << "\n";
    }
    return 0;
}