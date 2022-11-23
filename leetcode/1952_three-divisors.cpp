#include "common.h"


class Solution {
public:
    bool isThree(int n) {
        if (n < 3) return false;
        int root = sqrt(n);
        if (root * root != n) return false;
        for(int i = 2; i < root / 2; i ++) {
            if (!(root % i)) return false;
        }
        return true;
    }
};

void TestSolution() {
    Solution s;
    cout << s.isThree(81) << endl; 
}