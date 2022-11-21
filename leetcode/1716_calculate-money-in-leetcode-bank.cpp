#include "common.h"


class Solution {
public:
    int totalMoney(int n) {
        int res = 0;
        for(int i = 1; i <= n; i++) {
            res += (i - (i - 1) / 7 * 6);

        }
        return res;

    }
};
