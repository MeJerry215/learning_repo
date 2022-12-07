#include "common.h"

class Solution
{
public:
    int maxResult(vector<int> &nums, int k)
    {
        print_vec(nums);
        vector<int> dp(nums.size(), -0x3f3f3f3f);
        dp[0] = nums[0];
        for (int i = 1; i < nums.size(); i++)
        {
            for (int j = max(i - k, 0); j < i; j++)
            {
                dp[i] = max(dp[i], dp[j] + nums[i]);
            }
            print_vec(dp);
        }
        return dp[nums.size() - 1];
    }
};