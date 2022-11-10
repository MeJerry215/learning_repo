#include "common.h"


class Solution {
public:
    bool halvesAreAlike(string s) {
        unordered_set<char> vowels = {
            'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'
        };
        int i = 0, j = s.size() / 2, p = 0;
        while(j < s.size()) {
            if (vowels.count(s[i++])) p++;
            if (vowels.count(s[j++])) p--;
        }
        return p == 0;
    }
};