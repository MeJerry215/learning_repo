#include "common.h"

class Solution
{
public:
    vector<int> rearrangeArray(vector<int> &nums)
    {
        vector<int> res(nums.size());
        int postive_idx = 0;
        int negetive_idx = 0;
        int index = 0;
        while (postive_idx < nums.size() && negetive_idx < nums.size())
        {
            while (nums[postive_idx] < 0)
                postive_idx++;
            while (nums[negetive_idx] > 0)
                negetive_idx++;
            res[index++] = nums[postive_idx++];
            res[index++] = nums[negetive_idx++];
        }
        return res;
    }
};