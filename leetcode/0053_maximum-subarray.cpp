#include "common.h"

class Solution
{
public:
    int maxSubArray(vector<int> &nums)
    {
        int res = INT_MIN;
        int dp = 0;
        for (int i = 0; i < nums.size(); i++)
        {
            dp = max(dp + nums[i], nums[i]);
            res = max(res, dp);
        }

        return res;
    }
};


void TestSolution()
{
    Solution s;
    vector<int> nums = {5, 4, -1, 7, 8};
    cout << s.maxSubArray(nums) << endl;
}