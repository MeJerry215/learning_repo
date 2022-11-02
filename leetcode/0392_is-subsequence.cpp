#include "common.h"

class Solution
{
public:
    bool isSubsequence(string s, string t)
    {
        int i = 0;
        for (char c : t)
        {
            if (c == s[i])
                i++;
        }
        return s.size() == i;
    }
};