#include "common.h"

class Solution
{
public:
    bool arrayStringsAreEqual(vector<string> &word1, vector<string> &word2)
    {
        int i = 0, k = 0, l = 0;
        while(i < word1.size()) {
            for(int j = 0; j < word1[0].size(); j ++) {
                if (k < word2.size() && l < word2[0].size()) {
                    if (word1[i][j] == word2[k][l]) {

                    } else {

                    }
                } else if (k < word2.size()) {
                    k ++;
                } else if (k == word2.size()) {
                    return false;
                }
            }
        }
        return true;
    }
};