#include "common.h"

class Solution {
public:
    int monotoneIncreasingDigits(int n)
    {
        string num_str = to_string(n);
        for (int i = num_str.size() - 2; i >= 0; i--)
        {
            if (num_str[i] > num_str[i + 1])
            {
                num_str[i]--;
                // num_str[i + 1] = '9';
                fill(num_str.begin() + i + 1, num_str.end(), '9');
            }
        }
        return atoi(num_str.c_str());
    }
};