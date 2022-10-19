#include "common.h"

class Solution
{
public:
    int dominantIndex(vector<int> &nums)
    {
        int max_val = *max_element(nums.begin(), nums.end());
        int max_idx = -1;
        for (int i = 0; i < nums.size(); i++)
        {
            if (max_val == nums[i])
            {
                max_idx = i;
                continue;
            }
            if (max_val < nums[i] * 2)
                return -1;
        }
        return max_idx;
    }
};