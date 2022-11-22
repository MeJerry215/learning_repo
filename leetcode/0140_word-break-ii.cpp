#include "common.h"
/*
给定一个字符串 s 和一个字符串字典wordDict，在字符串s中增加空格来构建一个句子，使得句子中所有的单词都在词典中。以任意顺序 返回所有这些可能的句子。
注意：词典中的同一个单词可能在分段中被重复使用多次。

示例 1：
输入:s = "catsanddog", wordDict = ["cat","cats","and","sand","dog"]
输出:["cats and dog","cat sand dog"]
示例 2：

输入:s = "pineapplepenapple", wordDict = ["apple","pen","applepen","pine","pineapple"]
输出:["pine apple pen apple","pineapple pen apple","pine applepen apple"]
解释: 注意你可以重复使用字典中的单词。
示例3：
输入:s = "catsandog", wordDict = ["cats","dog","sand","and","cat"]
输出:[]

提示：
1 <= s.length <= 20
1 <= wordDict.length <= 1000
1 <= wordDict[i].length <= 10
s和wordDict[i]仅有小写英文字母组成
wordDict中所有字符串都 不同

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/word-break-ii
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution
{
public:
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        vector<vector<string>> dp(s.size() + 1);
        unordered_set<string> words(wordDict.begin(), wordDict.end());
        dp[0].push_back("");
        int min_len = INT_MAX;
        for (auto& word : wordDict) {
            int len = word.size();
            min_len = min(min_len, len);
        }
        for (int i = min_len; i <= s.size(); i++) {
            for (auto& word : wordDict) {
                int start = i - word.size();
                if (start < 0) continue;
                if (!dp[i - word.size()].empty() && s.substr(start, word.size()) == word) {
                    for (auto& sub : dp[start]) {
                        if (start == 0) {
                            dp[i].push_back(word);
                        } else {
                            dp[i].push_back(sub + " " + word);
                        }
                    }
                }
            }
        }

        return dp[s.size()];
    }
};

