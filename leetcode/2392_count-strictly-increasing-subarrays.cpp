#include "common.h"


class Solution {
public:
    long long countSubarrays(vector<int>& nums) {
        long long res = 1;
        int count = 1;
        for(int i = 1; i < nums.size(); i++) {
            if (nums[i] <= nums[i - 1]) {
                count = 0;
            }
            count ++;
            res += count;
        }
        return res;
    }
};

