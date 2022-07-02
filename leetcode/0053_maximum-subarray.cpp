#include "common.h"

class Solution
{
public:
    int maxSubArray(vector<int> &nums)
    {
        int sum = 0;
        int max_val = INT_MIN;
        for (int i = 0; i < nums.size(); i++)
        {
            sum += nums[i];
            max_val = max(max_val, sum);
            if (sum < 0)
            {
                sum = 0;
            }
        }
        return max_val;
    }
};

void TestSolution()
{
    Solution s;
    vector<int> nums = {5, 4, -1, 7, 8};
    cout << s.maxSubArray(nums) << endl;
}