#include "common.h"

class Solution
{
public:
    vector<string> ambiguousCoordinates(string s)
    {
        vector<string> res;
        s = s.substr(1, s.size() - 2);
        function<vector<string>(string str)> find_all_sub_str = [&](string str) -> vector<string>
        {
            vector<string> strs;
            if (str[0] != '0' || str == "0")
                strs.push_back(str);
            for (int i = 1; i < str.size(); i++)
            {
                if (str.back() == '0' || (str[0] == '0' && i > 1))
                    continue;
                strs.push_back(str.substr(0, i) + "." + str.substr(i));
            }
            return strs;
        };
        for (int i = 1; i < s.size(); i++)
        {
            vector<string> lstrs = find_all_sub_str(s.substr(0, i));
            if (lstrs.empty())
                continue;
            vector<string> rstrs = find_all_sub_str(s.substr(i));
            if (rstrs.empty())
                continue;
            for (auto &lstr : lstrs)
            {
                for (auto &rstr : rstrs)
                {
                    res.push_back("(" + lstr + ", " + rstr + ")");
                }
            }
        }
        return res;
    }
};