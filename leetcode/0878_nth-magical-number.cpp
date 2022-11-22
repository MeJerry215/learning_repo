#include "common.h"

/*
一个正整数如果能被 a 或 b 整除，那么它是神奇的。
给定三个整数 n , a , b ，返回第 n 个神奇的数字。因为答案可能很大，所以返回答案 对 109 + 7 取模 后的值。

示例 1：
输入：n = 1, a = 2, b = 3
输出：2
示例 2：
输入：n = 4, a = 2, b = 3
输出：6

提示：
1 <= n <= 109
2 <= a, b <= 4 * 104
 

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/nth-magical-number
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    int gcd(int x, int y) {
        return y > 0 ? gcd(y, x % y) : x;
    }
    int lcm(int x, int y) {
        return x * y / gcd(x, y);
    }

    int nthMagicalNumber(int n, int a, int b) {
        const int MOD = 1e9 + 7;
        long long l = min(a, b), r = n * l;
        int g = lcm(a, b);

        while (l <= r) {
            long long mid = l + (r - l) / 2;
            int k = mid / a + mid / b - mid / g;
            if (k >= n) {
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }
        return (r + 1) % MOD;
    }
};