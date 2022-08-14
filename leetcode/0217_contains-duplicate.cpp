#include "common.h"

// class Solution
// {
// public:
//     bool containsDuplicate(vector<int> &nums)
//     {
//         sort(nums.begin(), nums.end());
//         for (int i = 1; i < nums.size(); i++)
//         {
//             if (nums[i] == nums[i - 1])
//             {
//                 return true;
//             }
//         }
//         return false;
//     }
// };

class Solution
{
public:
    bool containsDuplicate(vector<int> &nums)
    {
        unordered_set<int> num_set;
        for (auto &num : nums)
        {
            if (num_set.count(num) > 0)
            {
                return true;
            }
            num_set.insert(num);
        }

        return false;
    }
};

void TestSolution()
{
}