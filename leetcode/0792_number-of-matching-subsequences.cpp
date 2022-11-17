#include "common.h"

/*
给定字符串 s和字符串数组words, 返回words[i]中是s的子序列的单词个数。

字符串的 子序列
是从原始字符串中生成的新字符串，可以从中删去一些字符(可以是none)，而不改变其余字符的相对顺序。

例如， “ace” 是 “abcde” 的子序列。


示例 1:

输入: s = "abcde", words = ["a","bb","acd","ace"]
输出: 3
解释: 有三个是s 的子序列的单词: "a", "acd", "ace"。
Example 2:

输入: s = "dsahjpjauf", words = ["ahjpjau","ja","ahbwzgqnuk","tnmlanowax"]
输出: 2


提示:

1 <= s.length <= 5 * 104
1 <= words.length <= 5000
1 <= words[i].length <= 50
words[i]和 s都只由小写字母组成。

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/number-of-matching-subsequences
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution
{
public:
    int numMatchingSubseq(string s, vector<string> &words)
    {
        vector<int> pos[26];
        for (int i = 0; i < s.size(); i++)
        {
            pos[s[i] - 'a'].push_back(i);
        }
        int res = 0;
        for (string &word : words)
        {
            int idx = -1;
            bool is_done = true;
            for (char c : word)
            {
                auto iter = upper_bound(pos[c - 'a'].begin(), pos[c - 'a'].end(), idx);
                if (iter == pos[c - 'a'].end())
                {
                    is_done = false;
                    break;
                }
                idx = *iter;
            }
            if (is_done)
            {
                cout << word << " is found" << endl;
                res++;
            }
        }
        return res;
    }

    int numMatchingSubseq(string s, vector<string> &words)
    {
        queue<pair<int, int>> matches[26];
        for (int i = 0; i < words.size(); i++)
        {
            matches[words[i][0] - 'a'].emplace(i, 0);
        }
        int res = 0;
        for (auto c : s)
        {
            queue<pair<int, int>> &match = matches[c - 'a'];
            int n = match.size();
            for (int i = 0; i < n; i++)
            {
                pair<int, int> &pos = match.front();
                pos.second += 1;
                if (pos.second == words[pos.first].size())
                {
                    res++;
                    cout << words[pos.first] << " is found" << endl;
                }
                else
                {
                    matches[words[pos.first][pos.second] - 'a'].emplace(pos.first,
                                                                        pos.second);
                }
                match.pop();
            }
        }
        return res;
    }
};