#include "common.h"

class Solution
{
public:
    int maxLengthBetweenEqualCharacters(string s)
    {
        int last_postion[26];
        fill(begin(last_postion), end(last_postion), -1);
        int res = -1;

        for (int i = 0; i < s.size(); i++)
        {
            if (last_postion[s[i] - 'a'] != -1)
            {
                res = max(res, i - last_postion[s[i] - 'a'] - 1);
            }
            else
            {
                last_postion[s[i] - 'a'] = i;
            }
        }

        return res;
    }
};