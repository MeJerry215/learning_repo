# 动态规划
动态规划是指把一个问题分成若干个子问题，通过局部最优解得到全局最优的一种算法策略或者说一种思想方法，简单来讲就是用一个数组表示我们要求的问题的答案：由前一个问题的答案推出另一个问题的答案。
dp一般有三个步骤：
1、设计状态：设计合适的数组以及相应的含义。
2、状态转移方程：从已知问题的答案推出当前问题的答案。
3、确定边界条件：递推的初值或者说记忆化搜索的回溯条件，以及各个数组的初值。

https://github.com/SharingSource/LogicStack-LeetCode/wiki/%E8%AE%B0%E5%BF%86%E5%8C%96%E6%90%9C%E7%B4%A2

- 记忆化搜索
- 线性dp
- 背包dp
- 序列dp
- 区间dp
- 状态压缩dp
- 状态机dp
- 数位dp
- 树形dp


[5. 最长回文子串](https://leetcode.cn/problems/longest-palindromic-substring/)
[62. 不同路径](https://leetcode.cn/problems/unique-paths/)
[63. 不同路径 II](https://leetcode.cn/problems/unique-paths-ii/)
[1235. 规划兼职工作](https://leetcode.cn/problems/maximum-profit-in-job-scheduling/)

## 线性dp
在线性结构上进行状态转移，目标函数为特定变量的线性函数，目的是求目标函数的最大值或最小值。

## 最长回文子串

```
给你一个字符串 s，找到 s 中最长的回文子串。

示例 1：

输入：s = "babad"
输出："bab"
解释："aba" 同样是符合题意的答案。
示例 2：

输入：s = "cbbd"
输出："bb"
 

提示：

1 <= s.length <= 1000
s 仅由数字和英文字母组成

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/longest-palindromic-substring
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
```

本题目最简单的思路是遍历每一位，判断最长子串。

如果要考虑用动态规划做，那么考虑一下如何进行状态转移

存在任意子字符串S[i,j], 如果S[i, j] 是回文串，则S[i] == S[j] 且 S[i + 1, j - 1] 也是回文串。对于任意的 S[i, i]必然是回文


```c++
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
```

## 不同路径

62 不同路径
```
一个机器人位于一个 m x n 网格的左上角 （起始点在下图中标记为 “Start” ）。

机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为 “Finish” ）。

问总共有多少条不同的路径？

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/unique-paths
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
```
由于只能往下或者网友走，所以dp[i, j] = dp[i, j - 1] + dp[i - 1, j]


63 unique-paths-ii

加入了障碍，就导致遇到障碍之后重置为0

```c++
int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
    int n = obstacleGrid.size(), m = obstacleGrid.at(0).size();
    vector <int> f(m);

    f[0] = (obstacleGrid[0][0] == 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (obstacleGrid[i][j] == 1) {
                f[j] = 0;
                continue;
            }
            if (j - 1 >= 0 && obstacleGrid[i][j - 1] == 0) {
                f[j] += f[j - 1];
            }
        }
    }

    return f.back();
}
```