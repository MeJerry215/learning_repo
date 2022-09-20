#include "common.h"

class Solution
{
public:
    bool areNumbersAscending(string s)
    {
        int pre = -1;
        int start, end;
        for (int i = 0; i < s.size(); i++)
        {
            if (isdigit(s[i]))
            {
                start = i;
                end = start + 1;
                while (end < s.size() && isdigit(s[end]))
                    end++;
                int val = atoi(s.substr(start, end - start).c_str());
                if (val <= pre)
                    return false;
                pre = val;
                i = end - 1;
            }
        }
        return true;
    }
};