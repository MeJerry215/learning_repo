#include "common.h"

class Solution {
public:
    bool dfs_is_interleave(string& s1, string& s2, string& s3, int i, int j, int k) {
        if (s1[i] != s3[k] && s2[j] != s3[k]) {
            return false;
        }
        if (k == s3.size()) {
            return true;
        }
        bool ret = false;
        if (i < s1.size() && s1[i] == s3[k]) {
            ret = dfs_is_interleave(s1, s2, s3, i + 1, j, k + 1);
        }
        if (ret) return ret;
        if (j < s2.size() && s2[j] == s3[k]) {
            ret = dfs_is_interleave(s1, s2, s3, i, j + 1, k + 1);
        }
        if (ret) return ret;
        return false;
    }

    bool isInterleave_v1(string s1, string s2, string s3) {
        if (s1.size() + s2.size() != s3.size()) {
            return false;
        }
        return dfs_is_interleave(s1, s2, s3, 0, 0, 0);
    }

    bool isInterleave(string s1, string s2, string s3) {
        if (s1.size() + s2.size() != s3.size()) {
            return false;
        }
        if (s3.size() == 0) {
            return true;
        }
        s1.size() < s2.size() ? s1.append("a") : s2.append("a");
        s3 += "a";

        bool dp[s1.size() + 1][s2.size() + 1];
        // 0, 0 必然能够组成s3 的0个元素
        dp[0][0] = true;
        for (int i = 0; i <= s1.size(); i++) {
            for(int j = 0; j <= s2.size(); j++) {
                if (i == 0 && j == 0) continue;
                dp[i][j] = (i - 1 >= 0 && dp[i - 1][j] && s1[i - 1] == s3[i + j - 1]) ||
                 (j - 1 >= 0 &&dp[i][j - 1] && s2[j - 1] == s3[i + j - 1]);
            }
            cout << endl;
        }
        return dp[s1.size()][s2.size()];
    }
};