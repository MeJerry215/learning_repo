#include "common.h"

class Solution
{
public:
    vector<int> smallerNumbersThanCurrent_v1(vector<int> &nums)
    {
        int counts[101] = {0};
        for (auto num : nums)
        {
            counts[num]++;
        }
        int count = 0;
        vector<int> res;
        for (int i = 0; i < 101; i++)
        {
            if (counts[i])
            {
                int sum = count + counts[i];
                counts[i] = count;
                count = sum;
            }
        }
        for (auto num : nums)
        {
            res.push_back(counts[num]);
        }
        return res;
    }
}