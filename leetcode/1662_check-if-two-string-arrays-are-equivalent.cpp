#include "common.h"

class Solution
{
public:
    bool arrayStringsAreEqual(vector<string> &word1, vector<string> &word2)
    {
        int i = 0, k = 0, l = 0;
        while(i < word1.size()) {
            for(int j = 0; j < word1[i].size(); j ++) {
                if (k < word2.size() && l < word2[k].size()) {
                    if (word1[i][j] == word2[k][l]) {
                        l++;
                    } else {
                        return false;
                    }
                } else if (k < word2.size()) {
                    k ++;
                    l = 0;
                    j --;
                } else if (k == word2.size()) {
                    return false;
                }
            }
            i++;
        }
        if (k == word2.size() - 1 && l == word2[k].size()) return true;
        return false;
    }
};