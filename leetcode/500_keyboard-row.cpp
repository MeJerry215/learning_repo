#include "common.h"


class Solution {
public:
    vector<string> findWords(vector<string>& words) {
        vector<string> ans;
        string rowIdx = "12210111011122000010020202";
        for (auto & word : words) {
            bool is_valid = true;
            char idx = rowIdx[tolower(word[0]) - 'a'];
            for (int i = 1; i < word.size(); ++i) {
                if(rowIdx[tolower(word[i]) - 'a'] != idx) {
                    is_valid = false;
                    break;
                }
            }
            if (is_valid) {
                ans.emplace_back(word);
            }
        }
        return ans;
    }
};

void TestSolution() {
    Solution s;
    string tmp = "qwertyuiop";
        for (int c : tmp) {
            cout << c << " ";
        }
        cout << endl; 
}