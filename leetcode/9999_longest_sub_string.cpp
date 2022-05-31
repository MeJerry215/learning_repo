#include "common.h"

class Solution
{
public:
    string longestCommonPrefix(vector<string> &strs)
    {
        string ans = "";
        size_t min_len = INT_MAX;
        for_each(strs.begin(), strs.end(), [&min_len](const string &str)
                 { min_len = min(min_len, str.size()); });
        if (min_len == INT16_MAX || min_len == 0)
        {
            return ans;
        }
        if (strs.size() == 1)
        {
            return strs[0];
        }
        for (size_t i = 1; i <= min_len; i++)
        {
            string sub = strs[0].substr(0, i);
            if (all_of(strs.begin(), strs.end(), [&sub](const string &str)
                       { return str.find(sub) == 0; }))
            {
                ans = sub;
                continue;
            };
            return ans;
        }
        return ans;
    }
};

void TestSolution()
{
    Solution s;
    vector<string> strs = {"flower", "flow", "flight"};
    auto ans = s.longestCommonPrefix(strs);
    cout << ans << endl;
    strs = {"dog", "racecar", "car"};
    ans = s.longestCommonPrefix(strs);
    cout << ans << endl;
    strs = {"c", "acc", "ccc"};
    ans = s.longestCommonPrefix(strs);
    cout << ans << endl;
}