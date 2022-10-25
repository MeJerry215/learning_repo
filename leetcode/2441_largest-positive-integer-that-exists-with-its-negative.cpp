#include "common.h"


class Solution {
public:
    int findMaxK(vector<int>& nums) {
        sort(nums.begin(), nums.end(), [](const int lhs, const int rhs) {
            return abs(lhs) > abs(rhs);
        });
        for(int i = 0; i < nums.size() - 1; i++) {
            if (nums[i] + nums[i + 1] == 0) return abs(nums[i]);
        }
        return -1;
    }
};