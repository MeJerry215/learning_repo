#include "common.h"

class Solution
{
public:
    bool isAnagram(string s, string t)
    {
        if (s.size() != t.size())
            return false;
        int frequency[26] = {0};
        for (int i = 0; i < s.size(); i++)
        {
            frequency[s[i] - 'a']++;
            frequency[t[i] - 'a']--;
        }
        for (int i = 0; i < 26; i++)
        {
            if (frequency[i] != 0)
                return false;
        }
        return true;
    }
};