.
#ininclude "ccommon.h"

    /*
    给定两个字符串s1 和 s2，返回 使两个字符串相等所需删除字符的 ASCII 值的最小和 。

    示例 1:
    输入: s1 = "sea", s2 = "eat"
    输出: 231
    解释: 在 "sea" 中删除 "s" 并将 "s" 的值(115)加入总和。
    在 "eat" 中删除 "t" 并将 116 加入总和。
    结束时，两个字符串相等，115 + 116 = 231 就是符合条件的最小和。
    示例 2:

    输入: s1 = "delete", s2 = "leet"
    输出: 403
    解释: 在 "delete" 中删除 "dee" 字符串变成 "let"，
    将 100[d]+101[e]+101[e] 加入总和。在 "leet" 中删除 "e" 将 101[e] 加入总和。
    结束时，两个字符串都等于 "let"，结果即为 100+101+101+101 = 403 。
    如果改为将两个字符串转换为 "lee" 或 "eet"，我们会得到 433 或 417 的结果，比答案更大。

    提示:

    0 <= s1.length, s2.length <= 1000
    s1 和 s2 由小写英文字母组成

    来源：力扣（LeetCode）
    链接：https://leetcode.cn/problems/minimum-ascii-delete-sum-for-two-strings
    著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
    */

    class Solution
{
public:
    /*
s1    1  3  4  5  7
s2    3  3  2  4  5
      j
      0   1   2   3   4   5
i 0   0   3   6   8   12  17
  1   1   4   7   9   13  18
  2   4   1   4   6   10  15
  3   8   5   8   10  6   11
  4   13  10  13  15  11  6
  5   20  17  20  22  18  13
  */
    int minimumDeleteSum(string s1, string s2)
    {
        int m = s1.size();
        int n = s2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        /* if s2 is empty string caculate, from [0, i) range in the s1, char need to delete. */
        for (int i = 1; i <= m; i++)
        {
            dp[i][0] = dp[i - 1][0] + s1[i - 1];
        }
        /* if s1 is empty string, caculate the delete sum as well */
        for (int i = 1; i <= n; i++)
        {
            dp[0][i] = dp[0][i - 1] + s2[i - 1];
        }
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (s1[i - 1] == s2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1];
                }
                else
                {
                    dp[i][j] = min(dp[i - 1][j] + s1[i - 1], dp[i][j - 1] + s2[j - 1]);
                }
            }
        }
        return dp[m][n];
    }
};