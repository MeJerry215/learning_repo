#include "common.h"

class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        sort(strs.begin(), strs.end(), [](const string& lhs, const string& rhs) {
            return lhs.size() < rhs.size();
        });
        int i = strs[0].size();
        string& prefix = strs[0];
        for(int j = 1; j < strs.size(); j++) {
            string& toCompare = strs[j];
            for(int k = 0; k < i; k++) {
                if (prefix[k] == toCompare[k]) continue;
                i = k;
            }
        }

        return prefix.substr(0, i);
    }
};


void TestSolution() {
    Solution s;
    vector<string> strs = {
        "dog","racecar","car"
    };
    cout << s.longestCommonPrefix(strs) << endl;
}