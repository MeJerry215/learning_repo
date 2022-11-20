#include "common.h"

/*
给你一个字符串 s ，每个字符是数字 '1' 到 '9' ，再给你两个整数 k 和 minLength 。
如果对 s 的分割满足以下条件，那么我们认为它是一个 完美 分割：
s 被分成 k 段互不相交的子字符串。
每个子字符串长度都 至少 为 minLength 。
每个子字符串的第一个字符都是一个 质数 数字，最后一个字符都是一个 非质数 数字。质数数字为 '2' ，'3' ，'5' 和 '7' ，剩下的都是非质数数字。
请你返回 s 的 完美 分割数目。由于答案可能很大，请返回答案对 109 + 7 取余 后的结果。
一个 子字符串 是字符串中一段连续字符串序列。

示例 1：
输入：s = "23542185131", k = 3, minLength = 2
输出：3
解释：存在 3 种完美分割方案：
"2354 | 218 | 5131"
"2354 | 21851 | 31"
"2354218 | 51 | 31"
示例 2：
输入：s = "23542185131", k = 3, minLength = 3
输出：1
解释：存在一种完美分割方案："2354 | 218 | 5131" 。
示例 3：
输入：s = "3312958", k = 3, minLength = 1
输出：1
解释：存在一种完美分割方案："331 | 29 | 58" 。

提示：
1 <= k, minLength <= s.length <= 1000
s 每个字符都为数字 '1' 到 '9' 之一。

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/number-of-beautiful-partitions
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
    const int MOD = 1e9 + 7;

    bool is_prime(char c) {
        return c == '2' || c == '3' || c == '5' || c == '7';
    }

    // 判断是否可以在 j-1 和 j 之间分割（开头和末尾也算）
    bool can_partition(string &s, int j) {
        return j == 0 || j == s.length() || !is_prime(s[j - 1]) && is_prime(s[j]);
    }

public:
    int beautifulPartitions(string &s, int k, int l) {
        int n = s.length();
        if (k * l > n || !is_prime(s[0]) || is_prime(s[n - 1])) // 剪枝
            return 0;
        int f[k + 1][n + 1]; memset(f, 0, sizeof(f));
        f[0][0] = 1;
        for (int i = 1; i <= k; ++i) {
            int sum = 0;
            // 优化：枚举的起点和终点需要给前后的子串预留出足够的长度
            for (int j = i * l; j + (k - i) * l <= n; j++) {
                if (can_partition(s, j - l)) sum = (sum + f[i - 1][j - l]) % MOD; // j'=j-l 双指针
                if (can_partition(s, j)) f[i][j] = sum;
            }
        }
        return f[k][n];
    }
};
