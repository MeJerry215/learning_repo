#include "common.h"

class Solution
{
public:
    string removeStars(string s)
    {
        int idx = 0;
        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] == '*')
            {
                if (idx > 0)
                {
                    idx--;
                }
            }
            else
            {
                s[idx++] = s[i];
            }
        }
        cout << idx << endl;
        return s.substr(0, idx);
    }
};