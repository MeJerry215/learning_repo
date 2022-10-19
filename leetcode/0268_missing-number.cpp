#include "common.h"

class Solution
{
public:
    int missingNumber(vector<int> &nums)
    {
        int xor_val = 0;
        int n = nums.size();
        for (auto num : nums)
        {
            xor_val ^= num;
        }
        for (int i = 0; i <= n; i++)
        {
            xor_val ^= i;
        }
        return xor_val;
    }
};