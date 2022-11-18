#include "common.h"

class Solution
{
public:
    vector<string> findRepeatedDnaSequences(string s)
    {
        vector<string> res;
        if (s.size() <= 10)
            return {};
        unordered_map<string, int> counts;
        for (int i = 0; i <= s.size() - 10; i++)
        {
            string dna = s.substr(i, 10);
            if (counts[dna]++ == 1)
            {
                res.push_back(dna);
            }
        }
        return res;
    }

    const int L = 10;
    unordered_map<char, int> bin = {{'A', 0}, {'C', 1}, {'G', 2}, {'T', 3}};

public:
    vector<string> findRepeatedDnaSequences(string s)
    {
        vector<string> ans;
        int n = s.length();
        if (n <= L)
        {
            return ans;
        }
        int x = 0;
        for (int i = 0; i < L - 1; ++i)
        {
            x = (x << 2) | bin[s[i]];
        }
        unordered_map<int, int> cnt;
        for (int i = 0; i <= n - L; ++i)
        {
            x = ((x << 2) | bin[s[i + L - 1]]) & ((1 << (L * 2)) - 1);
            if (++cnt[x] == 2)
            {
                ans.push_back(s.substr(i, L));
            }
        }
        return ans;
    }
};
