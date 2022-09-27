#include "common.h"

class Solution {
public:
    enum {CHAR_NUM = 128};

    bool CheckPermutation(string s1, string s2) {
        int char_count[CHAR_NUM] = {0};
        for(const char c: s1) {
            char_count[c] ++;
        }
        for(const char c: s2) {
            char_count[c] --;
            if (char_count[c] < 0) return false;
        }
        return true;
    }
};