#include "common.h"

class Solution {
    const long MOD = 1e9 + 7;
public:
    int countPalindromes(string s) {
        int suf[10]{}, suf2[10][10]{}, pre[10]{}, pre2[10][10]{};
        for (int i = s.length() - 1; i >= 0; --i) {
            char d = s[i] - '0';
            for (int j = 0; j < 10; ++j)
                suf2[d][j] += suf[j];
            ++suf[d];
        }

        long ans = 0L;
        for (char d : s) {
            d -= '0';
            --suf[d];
            for (int j = 0; j < 10; ++j)
                suf2[d][j] -= suf[j]; // 撤销
            for (int j = 0; j < 10; ++j)
                for (int k = 0; k < 10; ++k)
                    ans += (long) pre2[j][k] * suf2[j][k]; // 枚举所有字符组合
            for (int j = 0; j < 10; ++j)
                pre2[d][j] += pre[j];
            ++pre[d];
        }
        return ans % MOD;
    }
};




void TestSolution() {
    Solution s;
    cout << s.countPalindromes("9999900000") << endl;
}