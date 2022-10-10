#include "common.h"

class Solution
{
public:
    string defangIPaddr(string address)
    {
        string res = "";
        for (char c : address)
        {
            if (isdigit(c))
            {
                res += c;
            }
            else
            {
                res += "[.]";
            }
        }
        return res;
    }
};