#include "common.h"

class Solution
{
public:
    vector<string> stringMatching(vector<string> &words)
    {
        // sort array with length, the small size string will be on the left, the large size string on
        // the right.
        int n = words.size();
        sort(words.begin(), words.end(), [](const string &lhs, const string &rhs)
             {
      if (lhs.size() == rhs.size()) {
        return lhs < rhs;
      } else {
        return lhs.size() < rhs.size();
      } });
        vector<string> ans;
        // match from tail words
        for (int i = 0; i < n; i++)
        {
            int j = n - 1;
            while (j > i && words[i].size() != words[j].size())
            {
                if (words[j].find(words[i]) != string::npos)
                {
                    ans.push_back(words[i]);
                    break;
                }
                j--;
            }
        }
        return ans;
    }
};