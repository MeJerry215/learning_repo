#include "common.h"


class Solution {
public:
    string removeKdigits(string num, int k) {
        stack<int> st;
        unordered_set<int> pos;
        for(int i = 0; i < num.size(); i++) {
            while (!st.empty() && num[st.top()] > num[i] && pos.size() != k) {
                pos.insert(st.top());
                st.pop();
            }
            if (pos.size() == k) break;
            st.push(i);
        }
        while (pos.size() < k) {
            pos.insert(st.top());
            st.pop();
        }
        string res;
        bool skip = true;
        for (int i = 0; i < num.size(); i++) {
            if (pos.count(i)) continue;
            if (num[i] != '0') skip = false;
            if (skip && num[i] == '0') continue;
            res.push_back(num[i]);
        }
        return res.size() ? res : "0";
    }
};


void Solution()