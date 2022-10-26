#include "common.h"

class Solution
{
public:
    int mySqrt(int x)
    {
        int low = 0, high = x;
        int res = 0;
        while (low <= high)
        {
            int mid = (low + high) / 2;
            if (mid != 0 && INT_MAX / mid < mid)
            {
                high = mid - 1;
                continue;
            }
            if (mid * mid == x)
                return mid;
            if (mid * mid < x)
            {
                res = mid;
                low = mid + 1;
            }
            else
            {
                high = mid - 1;
            }
        }
        return res;
    }
};