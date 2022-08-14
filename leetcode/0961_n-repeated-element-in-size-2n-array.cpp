#include "common.h"
class Solution
{
public:
    int repeatedNTimes(vector<int> &nums)
    {
        int i;
        if (nums[1] == nums[3])
            return nums[1];
        for (i = 0; i < nums.size() - 1; i++)
            if (nums[i] == nums[i + 1])
                return nums[i];
        return nums[0];
    }
};