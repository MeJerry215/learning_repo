#include "common.h"

class Solution
{
public:
    vector<int> plusOne(vector<int> &digits)
    {
        bool carry = true;
        int n = digits.size() - 1;
        while (n >= 0)
        {
            digits[n] += (carry ? 1 : 0);
            carry = digits[n] / 10;
            digits[n--] %= 10;
        }
        if (carry)
            digits.insert(digits.begin(), 1);
        return digits;
    }
};