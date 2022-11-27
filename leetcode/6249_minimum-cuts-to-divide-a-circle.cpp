#include "common.h"


class Solution {
public:
    int numberOfCuts(int n) {
        if (n < 2) return 0;
        if (n % 2) {
            return n;
        } else {
            return n / 2;
        }
    }
};