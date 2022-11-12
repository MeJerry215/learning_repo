#include "common.h"


class Solution {
    const int MOD = 1e9 + 7;
public:
    int numTilings(int n) {
        if (n == 1) return 1;
        long a = 1, b = 1, c = 2;
        for (int i = 3; i <= n; ++i) {
            long f = (c * 2 + a) % MOD;
            a = b;
            b = c;
            c = f;
        }
        return c;
    }
};