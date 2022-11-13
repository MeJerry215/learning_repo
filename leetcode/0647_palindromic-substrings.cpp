#include "common.h"

/*
给你一个字符串 s ，请你统计并返回这个字符串中 回文子串 的数目。
回文字符串 是正着读和倒过来读一样的字符串。
子字符串 是字符串中的由连续字符组成的一个序列。
具有不同开始位置或结束位置的子串，即使是由相同的字符组成，也会被视作不同的子串。

示例 1：
输入：s = "abc"
输出：3
解释：三个回文子串: "a", "b", "c"
示例 2：
输入：s = "aaa"
输出：6
解释：6个回文子串: "a", "a", "a", "aa", "aa", "aaa"

提示：

1 <= s.length <= 1000
s 由小写英文字母组成

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/palindromic-substrings
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    int countSubstrings(string s) {
        int n = s.size();
        bool dp[n][n];
        memset(&dp[0][0], 0, n * n);
        int res = 0;
        for(int i = 0; i < n; i++) {
            res ++;
            dp[i][i] = true;
        }
        for(int i = 1; i < n; i++) {
            for(int j = 0; j < i; j++) {
                if (s[i] != s[j]) dp[j][i] = false;
                else {
                    if (i - j < 2 || dp[j + 1][i - 1]) {
                        dp[j][i] = true;
                        res ++;
                    }
                    else dp[j][i] = false;
                }
            }
        }
        return res;
    }

    int countSubstrings(string s) {
        int n = s.size(), ans = 0;
        for (int i = 0; i < 2 * n - 1; ++i) {
            int l = i / 2, r = i / 2 + i % 2;
            while (l >= 0 && r < n && s[l] == s[r]) {
                --l;
                ++r;
                ++ans;
            }
        }
        return ans;
    }


};


