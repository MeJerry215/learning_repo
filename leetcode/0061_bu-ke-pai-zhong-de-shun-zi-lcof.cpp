#include "common.h"

class Solution {
public:
    bool isStraight(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int joker = 0;
        for(int i = 0; i < nums.size() - 1; i++) {
            if (nums[i] == 0) joker ++;
            else if (nums[i] == nums[i + 1]) return false;
        }
        return nums[4] - nums[joker] < 5;
    }
};