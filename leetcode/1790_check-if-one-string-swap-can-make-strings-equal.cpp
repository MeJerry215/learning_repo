#include "common.h"

class Solution {
public:
    bool areAlmostEqual(string s1, string s2) {
        int pre = -1, next = -1;
        for(int i = 0; i < s1.size(); i++) {
            if (s1[i] != s2[i]) {
                if (pre == -1) {
                    pre = i;
                } else if (next == -1) {
                    next = i;
                } else {
                    return false;
                }
            }
        }
        if ((pre ^ next) == 0) return true;
        if (pre == -1 || next == -1) return false;
        if (s1[pre] == s2[next] && s1[next] == s2[pre]) return true;
        return false;
    }
};