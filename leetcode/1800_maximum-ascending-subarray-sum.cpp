#include "common.h"


class Solution {
public:
    int maxAscendingSum(vector<int>& nums) {
        int sum = nums[0];
        int res = sum;
        for(int i = 1; i < nums.size(); i++) {
            if (nums[i] <= nums[i - 1]) {
                sum = 0;
            }
            sum += nums[i];
            res = max(res, sum);
        }
        return res;
    }
};