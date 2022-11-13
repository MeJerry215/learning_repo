#include "common.h"

class Solution {
public:
    int maxPalindromes(string s, int k) {
        int n = s.length(), f[n + 1];
        memset(f, 0, sizeof(f));
        for (int i = 0; i < 2 * n - 1; ++i) {
            int l = i / 2, r = l + i % 2; // 中心扩展法
            f[l + 1] = max(f[l + 1], f[l]);
            for (; l >= 0 && r < n && s[l] == s[r]; --l, ++r)
                if (r - l + 1 >= k)
                    f[r + 1] = max(f[r + 1], f[l] + 1);
        }
        return f[n];
    }
};



void TestSolution() {
    Solution s;
    string str = "fttfjofpnpfydwdwdnns";
    cout << s.maxPalindromes(str, 2) << endl;;
}