#include "common.h"

class Solution {
public:
    int newInteger(int n) {
        return n < 9 ? n :newInteger(n / 9) * 10 + n % 9;
    }
};


void TestSolution() {
    Solution s;
    cout << s.newInteger(9) << endl;

}