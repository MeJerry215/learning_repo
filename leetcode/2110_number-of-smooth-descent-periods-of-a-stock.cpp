#include "common.h"

/*
i wonder why this program cost too much time, I think the best of time cost will be O(n)
so doest it exsit any other optimize tech?
*/
class Solution {
public:
    long long getDescentPeriods(vector<int>& prices) {
        int i = 0;
        int j = 0;
        long long res = 0;
        int n = prices.size();
        while(j < n) {
            if (j == n - 1 || prices[j + 1] != prices[j] - 1) {
                long long nElement = j - i + 1;
                res += nElement * (nElement + 1) / 2;
                i = j + 1;
            }
            j ++;
        }
        return res;
    }
};


void TestSolution() {
    Solution s;
    vector<int> prices = {3,2,1,4};
    cout << s.getDescentPeriods(prices) << endl;
}