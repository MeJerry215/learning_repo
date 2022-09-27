#include "common.h"


class Solution {
public:
    int findMagicIndex(vector<int>& nums) {
        int i = 0;
        while(i < nums.size()) {
            if (nums[i] == i) return i;
            i = max(i + 1, nums[i]);
        }
        return -1;
    }
};