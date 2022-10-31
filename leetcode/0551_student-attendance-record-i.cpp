#include "common.h"

class Solution
{
public:
    bool checkRecord(string s)
    {
        int a_cnt = 0;
        int l_cnt = 0;
        for (const char c : s)
        {
            if (c == 'L')
            {
                l_cnt++;
                if (l_cnt == 3)
                    return false;
            }
            else
            {
                if (c == 'A')
                {
                    a_cnt++;
                    if (a_cnt == 2)
                        return false;
                }
                l_cnt = 0;
            }
        }
        return true;
    }
};