#include "common.h"

class Solution
{
public:
    vector<int> twoSum(vector<int> &nums, int target)
    {
        unordered_map<int, int> remains;
        for (int i = 0; i < nums.size(); i++)
        {
            int res = target - nums[i];
            auto it = remains.find(nums[i]);
            if (it != remains.end())
            {
                return {it->second, i};
            }
            remains[res] = i;
        }
        return {0, 0};
    }
};

void TestSolution()
{
}