#include "common.h"
/*
给你一个 32 位的有符号整数 x ，返回将 x 中的数字部分反转后的结果。
如果反转后整数超过 32 位的有符号整数的范围 [−231,  231 − 1] ，就返回 0。
假设环境不允许存储 64 位整数（有符号或无符号）。

示例 1：
输入：x = 123
输出：321
示例 2：
输入：x = -123
输出：-321
示例 3：
输入：x = 120
输出：21
示例 4：
输入：x = 0
输出：0
提示：
-231 <= x <= 231 - 1

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/reverse-integer
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution {
public:
    int reverse(int x) {
        if (x == INT_MIN) return 0;
        int sign = x >= 0 ? 1 : -1;
        x = abs(x);
        int ans = 0;
        while(x) {
            if (INT_MAX / 10 < ans) return 0;
            ans *= 10;
            if ((INT_MAX - x % 10) < ans) return 0;
            ans += x % 10;
            x /= 10;
        }
        return sign * ans;
    }
};