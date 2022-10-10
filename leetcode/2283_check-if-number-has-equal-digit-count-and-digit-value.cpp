#include "common.h"


class Solution
{
public:
    bool digitCount(string num)
    {
        const int NUM_LEN = 10;
        int num_counts[NUM_LEN] = {0};
        for (char c : num)
        {
            num_counts[c - '0']++;
        }
        for (int i = 0; i < num.size(); i++)
        {
            if (num_counts[i] != num[i] - '0')
                return false;
        }
        return true;
    }
};