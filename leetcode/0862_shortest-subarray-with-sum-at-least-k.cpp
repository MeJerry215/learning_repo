#include "common.h"
/*
0862
*/
class Solution
{
public:
    int shortestSubarray(vector<int> &nums, int k)
    {
        vector<long long> num_sums(nums.size() + 1, 0);
        for (int i = 0; i < nums.size(); i++)
        {
            num_sums[i + 1] = num_sums[i] + nums[i];
        }
        // print_vec(num_sums);
        deque<int> num_range;
        int res = INT_MAX;
        num_range.push_back(0);
        for (int i = 1; i < num_sums.size(); i++)
        {
            while (!num_range.empty() && num_sums[i] - num_sums[num_range.front()] >= k)
            {
                res = min(res, i - num_range.front());
                num_range.pop_front();
            }
            while (!num_range.empty() && num_sums[i] <= num_sums[num_range.back()])
            {
                num_range.pop_back();
            }
            num_range.push_back(i);
        }
        return res == INT_MAX ? -1 : res;
    }
};
