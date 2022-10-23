#include "common.h"

/*
0         0   0

1         1   1 - 1

2        10   1 -2
3        11   2

4       100   1 -4
5       101   2
6       110   2
7       111   3

8      1000   1 -8
9      1001   2
10     1010   2
11     1011   3
12     1100   2
13     1101   3
14     1110   3
15     1111   4

*/
class Solution {
public:
    vector<int> countBits(int n) {
        vector<int> dp(n + 1, 0);
        int base = 1;
        for(int i = 1; i <= n; i++) {
            if (i == 2 * base) base *= 2;
            dp[i] = dp[i - base] + 1;
        }
        return dp;
    }
};


