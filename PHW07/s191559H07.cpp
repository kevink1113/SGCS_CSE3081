#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits.h>

using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<ll> vl;

#define FOR(i, n) for(ll i = 0; i < n; i++)
#define FOR1(i, n) for(ll i = 1; i < n; i++)
ld res = LONG_LONG_MAX;
vl v1;
deque<ll> v2;


int main() {
    cin.tie(0);
    ios::sync_with_stdio(0);
    ll n;
    cin >> n;
    deque<ll> arr(n);
    FOR(i, n) cin >> arr[i];
    sort(arr.begin(), arr.end());

    while (1) {
        v1.push_back(arr.back());
        arr.pop_back();
        if (arr.empty()) break;

        v2.push_front(arr.front());
        arr.pop_front();
        if (arr.empty()) break;

        v1.push_back(arr.front());
        arr.pop_front();
        if (arr.empty()) break;

        v2.push_front(arr.back());
        arr.pop_back();
        if (arr.empty()) break;
    }

    FOR(i, v2.size()) v1.push_back(v2[i]);

    FOR(t, n) {
        ld ret = v1[0] + v1[n - 1];
        FOR1(i, n + 1) ret += 2 * sqrt(v1[i - 1] * v1[i]);
        res = min(res, ret);
        rotate(v1.begin(), v1.begin() + 1, v1.end());
    }
    cout << fixed << setprecision(9) << res << '\n';

    return 0;
}