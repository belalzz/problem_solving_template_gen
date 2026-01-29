#include <bits/stdc++.h>
using namespace std;

/*
    Problem: {{PROBLEM_NAME}}
    Author: {{AUTHOR}}
    Date: {{DATE}}

    Notes:
    - Input from stdin
    - Output to stdout
    - Time complexity matters
*/

// ---------- Type aliases ----------
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

// ---------- Constants ----------
constexpr int INF = 1e9;
constexpr ll LINF = 1e18;
constexpr int MOD = 1e9 + 7;

// ---------- Utility functions ----------
ll gcd(ll a, ll b) {
    return b == 0 ? a : gcd(b, a % b);
}

ll lcm(ll a, ll b) {
    return (a / gcd(a, b)) * b;
}

// ---------- Core solve logic ----------
void solve() {

}

// ---------- Main ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    // cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}
