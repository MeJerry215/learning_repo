#include "common.h"

class Solution
{
public:
    int fib(int n)
    {
        if (n < 2)
            return n;
        int p = 0, q = 1;
        int res = 0;
        for (int i = 2; i <= n; i++)
        {
            res = p + q;
            p = q;
            q = res;
        }
        return res;
    }
};