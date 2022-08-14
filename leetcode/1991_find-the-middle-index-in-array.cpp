#include "common.h"


class Solution
{
public:
    int findMiddleIndex(vector<int> &nums)
    {
        int res = accumulate(nums.begin(), nums.end(), 0);
        int rval = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (rval * 2 + nums[i] == res) return i;
            rval += nums[i];
        }
        return -1;
    }
};

void TestSolution()
{
    vector<int> nums = {2, 3, -1, 8, 4};
    Solution s;
    cout << s.findMiddleIndex(nums) << endl;
    nums = {1, -1, 4};
    cout << s.findMiddleIndex(nums) << endl;
    nums = {2, 5};
    cout << s.findMiddleIndex(nums) << endl;
    nums = {1};
    cout << s.findMiddleIndex(nums) << endl;
}