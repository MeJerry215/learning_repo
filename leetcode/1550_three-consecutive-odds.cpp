#include "common.h"

class Solution
{
public:
    bool threeConsecutiveOdds(vector<int> &arr)
    {
        for (int i = 1; i < arr.size() - 1; i++)
        {
            if (arr[i] % 2 == 0)
            {
                i++;
            }
            else if (arr[i + 1] % 2 == 0)
            {
                i += 2;
            }
            else if (arr[i - 1] % 2)
            {
                return true;
            }
        }
        return false;
    }
};