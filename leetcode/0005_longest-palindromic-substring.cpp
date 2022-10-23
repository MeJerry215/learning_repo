#include <iostream>
#include <string>
#include <vector>
#include "common.h"
using namespace std;

class Solution {
public:
    string longestPalindrome(string s) {
        int size = s.size();
        if (size < 2) {
            return s;
        }
        vector<vector<bool>> dp(size, vector<bool>(size));
        int dp[size][size];
        for (int i = 0; i < size; i++) {
            dp[i][i] = true;
        }
        int start = 0, len = 1;
        for (int j = 1; j < size; j++) {
            for(int i = 0; i < j; i++) {
                if (s[i] != s[j]) {
                    dp[i][j] = false;
                } else {
                    if (j - i < 3 || dp[i+1][j - 1]) {
                        dp[i][j] = true;
                    } else {
                        dp[i][j] = false;
                    }
                }
                if (dp[i][j] && (j - i + 1) > len) {
                    len = j - i + 1;
                    start = i;
                }
            }
        }
        return s.substr(start, len);
    }
};



void TestSolution() {
    Solution s;
    std::cout << s.longestPalindrome("abcaaaaaa") << std::endl;
}