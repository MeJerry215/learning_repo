#include "common.h"

class Solution {
public:
    bool canReach(string s, int minJump, int maxJump) {
        if (s.back() == '1') return false;
        queue<int> pos;
        pos.push(0);
        for(int i = 1; i < s.size(); i++) {
            if (s[i] == '1') continue;
            while(!pos.empty() && pos.front() < i - maxJump) {
                pos.pop();
            }
            if (pos.empty()) return false;
            if (pos.front() > i - minJump) {
                if (i == s.size() - 1) return false;
                continue;
            }
            pos.push(i);
        }
        return true;
    }
};
