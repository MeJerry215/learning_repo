#include "common.h"


class Solution {
public:
    int reverse(int v) {
        // 1031 = 1301
        int res = 0;
        while(v) {
            res *= 10;
            res +=(v % 10);
            v /= 10;
        }
        return res;
    }

    bool sumOfNumberAndReverse(int num) {
        for(int i = 0; i <= num; i++) {
            if (reverse(i) + i == num) {
                return true;
            }
        }
        return false;
    }
};