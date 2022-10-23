#include "common.h"


class Solution {
public:
    string mergeAlternately(string word1, string word2) {
        int k = min(word1.size(), word2.size());
        string res = "";
        for(int i = 0; i < k; i++) {
            res.push_back(word1[i]);
            res.push_back(word2[i]);
        }
        string& longger = word1.size() > word2.size() ? word1: word2;
        for(int i = k; k < longger.size(); i++) {
            res.push_back(longger[i]);
        }
        return res;
    }
};