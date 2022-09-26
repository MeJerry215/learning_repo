#include "common.h"


class Solution {
public:
    int sumFourDivisors(vector<int>& nums) {
        int ans = 0;
        for (int num: nums) {
            // factor_cnt: 因数的个数
            // factor_sum: 因数的和
            int factor_cnt = 0, factor_sum = 0;
            for (int i = 1; i * i <= num; ++i) {
                if (factor_cnt > 4) break;
                if (num % i == 0) {
                    ++factor_cnt;
                    factor_sum += i;
                    if (i * i != num) {   // 判断 i 和 num/i 是否相等，若不相等才能将 num/i 看成新的因数
                        ++factor_cnt;
                        factor_sum += num / i;
                    }
                }
            }
            if (factor_cnt == 4) {
                ans += factor_sum;
            }
        }
        return ans;
    }
};


void TestSolution() {
    Solution s;
    vector<int> nums = {1,2,3,4,5,6,7,8,9,10};
    cout << s.sumFourDivisors(nums) << endl;
}