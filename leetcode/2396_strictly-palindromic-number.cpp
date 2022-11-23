#include "common.h"


class Solution {
public:
    bool isStrictlyPalindromic(int n) {
        for (int i = 2; i <= n - 2; i++) {
            int val = 0, k = n;
            while(k) {
                val *= i;
                val += (k % i);
                k /= i;
            }
            if (val != n) return false;
        }
        return true;
    }
};

void TestSolution() {
    Solution s;
    for(int i = 10000; i < 1e9; i++) {
        if (s.isStrictlyPalindromic(i)) {
            cout << i << endl;
        }
    }
}