#include "common.h"


class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> words(wordDict.begin(), wordDict.end());
        unordered_set<int> word_lens;
        int min_len = INT_MAX;
        vector<bool> dp(s.size() + 1, false);
        dp[0] = true; // empty string result is true
        for(auto word: wordDict) {
            int len = word.size();
            min_len = min(min_len, len);
            word_lens.insert(len);
        }
        for(int i = min_len; i <= s.size(); i++) {
            for(auto word_len: word_lens) {
                int start = i - word_len;
                if (start < 0) continue;
                if (dp[i - word_len] && words.count(s.substr(start, word_len))) {
                    dp[i] = true;
                    break;
                }
            }
        }
        return dp[s.size()];
    }
};