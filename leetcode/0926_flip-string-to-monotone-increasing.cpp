#include "common.h"

/*
如果一个二进制字符串，是以一些 0（可能没有 0）后面跟着一些 1（也可能没有 1）的形式组成的，那么该字符串是 单调递增 的。
给你一个二进制字符串 s，你可以将任何 0 翻转为 1 或者将 1 翻转为 0 。
返回使 s 单调递增的最小翻转次数。

示例 1：
输入：s = "00110"
输出：1
解释：翻转最后一位得到 00111.
示例 2：
输入：s = "010110"
输出：2
解释：翻转得到 011111，或者是 000111。
示例 3：

输入：s = "00011000"
输出：2
解释：翻转得到 00000000。
 

提示：

1 <= s.length <= 105
s[i] 为 '0' 或 '1'

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/flip-string-to-monotone-increasing
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    int minFlipsMonoIncr_v1(string s) {
        int n = s.size();
        int dp[n + 1][2];
        memset(dp, 0, sizeof(dp));
        for(int i = 0; i < s.size(); i++) {
            dp[i + 1][0] = dp[i][0] + (s[i] == '1');
            dp[i + 1][1] = min(dp[i][0], dp[i][1]) + (s[i] == '0');
        }
        return min(dp[n][0], dp[n][1]);
    }

    int minFlipsMonoIncr(string s) {
        int p = 0, q = 0;
        for(int i = 0; i < s.size(); i++) {
            q = min(p, q) + (s[i] == '0');
            p = p + (s[i] == '1');
        }
        return min(p, q);
    }
};