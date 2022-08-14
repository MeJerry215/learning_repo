#include "common.h"

class Solution
{
public:
    int minMoves2(vector<int> &nums)
    {
        int ret = 0, i = 0, j = nums.size() - 1;
        sort(nums.begin(), nums.end());
        while (i < j)
            ret += nums[j--] - nums[i++];
        return ret;
    }
};

void TestSolution()
{
    Solution s;
    vector<int> nums = {1, 10, 2, 9};
    cout << s.minMoves2(nums) << endl;
    nums = {1, 2, 3};
    cout << s.minMoves2(nums) << endl;
    nums = {1, 0, 0, 8, 6};
    cout << s.minMoves2(nums) << endl;
}