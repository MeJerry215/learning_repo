#include "common.h"

class Solution {
public:
    bool is_valid(unordered_map<char, int>& cnts) {
        for(auto iter: cnts) {
            if (iter.second % 2) return false;
        }
        return true;
    }

    int findTheLongestSubstring(string s) {
        unordered_map<char, int> cnts;
        for(int i = 0; i < s.size(); i++) {
            if (s[i] == 'a' || s[i] == 'e' || s[i] == 'i' || s[i] == 'o' || s[i] == 'u') {
                cnts[s[i]] ++;
            }
        }
    }
};