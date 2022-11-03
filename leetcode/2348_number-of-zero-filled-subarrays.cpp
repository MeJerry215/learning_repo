#include "common.h"

class Solution
{
public:
    long long zeroFilledSubarray(vector<int> &nums)
    {
        int zero_cnt = 0;
        long long res = 0;
        for (int i = 0; i < nums.size(); i++)
        {
            if (nums[i] == 0)
                zero_cnt++;
            else
                zero_cnt = 0;
            res += zero_cnt;
        }
        return res;
    }
};