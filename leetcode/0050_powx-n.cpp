#include "common.h"


/*
实现 pow(x, n) ，即计算 x 的整数 n 次幂函数（即，xn ）。

示例 1：
输入：x = 2.00000, n = 10
输出：1024.00000
示例 2：

输入：x = 2.10000, n = 3
输出：9.26100
示例 3：

输入：x = 2.00000, n = -2
输出：0.25000
解释：2-2 = 1/22 = 1/4 = 0.25
 

提示：

-100.0 < x < 100.0
-231 <= n <= 231-1
-104 <= xn <= 104

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/powx-n
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution {
public:
    /*
        这道题目分析一下 3.0 ^ 5
        3.0 ^ 5 = 3.0 ^ 2 * 3.0 ^ 2 * 3.0 ^ 1
                = (3.0 ^ 1 * 3.0 ^ 1) * (3.0 ^ 1 * 3.0 ^ 1) * 3.0 ^ 1

    */

    double myPow_v1(double x, int n) {
        if (n == 0) return 1.0;
        if (x == 1.0) return x;
        long long i = n;
        function<double(long long)> dfs = [&](long long k) {
            if (k == 1) return x;
            double tmp = dfs(k / 2);
            return tmp * tmp * (k % 2 ? x : 1.0); 
        };
        double res =  dfs(abs(i));
        return n >= 0 ? res : 1.0 / res;
    }

    double quickMul(double x, long long N) {
        double ans = 1.0;
        // 贡献的初始值为 x
        double x_contribute = x;
        // 在对 N 进行二进制拆分的同时计算答案
        while (N > 0) {
            if (N % 2 == 1) {
                // 如果 N 二进制表示的最低位为 1，那么需要计入贡献
                ans *= x_contribute;
            }
            // 将贡献不断地平方
            x_contribute *= x_contribute;
            // 舍弃 N 二进制表示的最低位，这样我们每次只要判断最低位即可
            N /= 2;
        }
        return ans;
    }

    double myPow(double x, int n) {
        long long N = n;
        if (x == 1.0) return x;
        return N >= 0 ? quickMul(x, N) : 1.0 / quickMul(x, -N);
    }

};

void TestSolution() {
    Solution s;
    // cout << s.myPow(3.0, 2) <<endl;
    cout << (-3 % 2) << endl;
}