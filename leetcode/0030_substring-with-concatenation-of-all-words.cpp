#include "common.h"


class Solution
{
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        unordered_map<string, int> words_visit;
        for(auto& word: words) {
            words_visit[word]++;
        }
        int n = words.size();
        int m = words[0].size();
        function<bool(int,int)> dfs = [&](int start, int word_num) {
            if (word_num == n) return true;
            string word = s.substr(start, m);
            if (!words_visit.count(word) || words_visit[word] <= 0) return false;
            words_visit[word]--;
            word_num ++;
            bool ret = dfs(start + m, word_num);
            words_visit[word]++;
            return ret;
        };
        vector<int> res;
        for(int i = 0; i <= (int)s.size() - m * n; i++) {
            if (dfs(i, 0)) {
                res.push_back(i);
            }
        }
        return res;
    }
};