#include "common.h"


class Solution {
public:
    string removeDuplicates_v1(string s) {
        int cur = -1;
        for(int i = 0; i < s.size(); i++) {
            if (cur < 0) {
                s[++cur] = s[i];
                continue;
            }
            if (s[i] == s[cur]) {
                cur --;
            } else {
                s[++cur] = s[i];
            }
        }
        return cur >= 0 ? s.substr(0, cur + 1) : "";
    }

    string removeDuplicates(string s) {
        string st;
        for(char c: s) {
            if (!st.empty() && st.back() == c) {
                st.pop_back();
            } else {
                st.push_back(c);
            }
        }
        return st;
    }
};

void TestSolution() {
    Solution s;
    string str = "asdffqqdffsa";
    cout  << s.removeDuplicates(str) << endl;
}