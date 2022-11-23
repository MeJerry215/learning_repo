#include "common.h"


class Solution {
public:
    bool canConvertString(string s, string t, int k)
    {
        if (s.size() != t.size())
            return false;
        vector<int> counts(26, 0);
        for (int i = 0; i < s.size(); i++)
        {
            int diff = (t[i] - s[i] + 26) % 26;
            if (diff == 0)
                continue;
            if (diff + counts[diff] * 26 > k)
                return false;
            counts[diff]++;
        }
        return true;
    }
};