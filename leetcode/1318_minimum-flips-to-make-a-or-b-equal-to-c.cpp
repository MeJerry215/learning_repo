#include "common.h"

class Solution
{
public:
    int minFlips(int a, int b, int c)
    {
        bitset<32> a_bit(a), b_bit(b), c_bit(c);
        int res = 0;
        for (int i = 0; i < 32; i++)
        {
            if ((a_bit.test(i) || b_bit.test(i)) == c_bit.test(i))
            {
                continue;
            }
            else
            {
                if (a_bit.test(i) == b_bit.test(i) && a_bit.test(i))
                {
                    res += 2;
                }
                else
                {
                    res += 1;
                }
            }
        }
        return res;
    }
};