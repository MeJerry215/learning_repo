#include "common.h"


class Solution {
public:
    string oddString(vector<string>& words) {
        string word = words[0];
        function<bool(vector<int>&, string&)> compare = [&](vector<int>& diff, string &rhs) {
            for(int i = 1; i < rhs.size(); i++) {
                if ((diff[i - 1]) != (rhs[i] - rhs[i - 1])) return false;
            }
            return true;
        };
        vector<int> diff(word.size() - 1);
        for(int i = 1; i < word.size(); i++) {
            diff[i - 1] = word[i] - word[i - 1];
        }
        for(int i = 1; i < words.size(); i++) {
            if (!compare(diff, words[i])) {
                if (i > 1) {
                    return words[i];
                } else {
                    if (compare(diff, words[i + 1])) {
                        return words[i];
                    } else {
                        return word;
                    }
                }
            }
        }
        return word;
    }
};