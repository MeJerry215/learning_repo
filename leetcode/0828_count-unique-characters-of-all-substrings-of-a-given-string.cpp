#include "common.h"

class Solution {
public:

    int uniqueLetterString(string s) {
        unordered_map<char, vector<int>> char_pos;
        for(int i = 0; i < s.size(); i++) {
            char_pos[s[i]].push_back(i);
        }
        int ans = 0;
        auto iter = char_pos.begin();
        while(iter != char_pos.end()) {
            auto& pos = (iter++)->second;
            pos.push_back(s.size());
            int pre = -1;
            int cur = 0;
            int next = 0;
            for(int i = 0; i < pos.size() - 1; i++) {
                cur = pos[i];
                next = pos[i + 1];
                ans += (cur - pre) * (next - cur);
                pre = cur;
                cur = next;
            }
            
        }
        return ans;
    }
};


