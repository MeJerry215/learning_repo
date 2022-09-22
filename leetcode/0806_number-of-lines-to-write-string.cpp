#include "common.h"


class Solution {
public:
    vector<int> numberOfLines(vector<int> &widths, string s) {
        int cur = 0;
        int line = 1;
        for (const auto c : s) {
            int width = widths[c - 'a'];
            cur = width + cur;
            if (cur <= 100) {
                continue;
            } else {
                cur = width;
                line += 1;
            }
        }
        return {line, cur};
    }
};