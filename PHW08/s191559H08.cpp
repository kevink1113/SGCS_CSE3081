#include <iostream>
#include <cstring>
#include <algorithm>

#define FOR(i, n) for(int i=0; i<n; i++)
using namespace std;
int arr[55], n, t, sum = 0, num;
bool check[55];

bool cmp(int a, int b) { return a > b; }

bool dfs(int s, int l, int idx) {
    bool sign;
    if(!l) sign = 1;
    else sign = 0;
    if (s == num) return 1;
    for (int i = idx + 1; i < n; i++) {
        if (check[i]) continue;
        if (t == arr[i] + l) {
            check[i] = 1;
            if (dfs(s + 1, 0, -1)) return 1;
            check[i] = 0;
            return 0;
        } else if (t > arr[i] + l) {
            check[i] = 1;
            if (dfs(s, l + arr[i], i)) return 1;
            check[i] = 0;
            if (sign) return 0;
            while (arr[i] == arr[i + 1]) i++;
        }
    }

    return 0;
}

int main() {
    cin >> n;
    FOR(i, n) {
        cin >> arr[i];
        sum += arr[i];
    }
    sort(arr, arr + n, cmp);
    for (t = arr[0]; t <= sum; t++) {
        if (sum % t == 0) {
            num = sum / t;
            memset(check, 0, sizeof(check));
            if (dfs(1, 0, -1)) {
                cout << t << '\n';
                break;
            }
        }
    }

    return 0;
}