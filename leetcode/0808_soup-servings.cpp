#include "common.h"

/*
有A和B 两种类型的汤。一开始每种类型的汤有n毫升。有四种分配操作：
提供 100ml 的 汤A 和 0ml 的 汤B 。
提供 75ml 的 汤A 和 25ml 的 汤B 。
提供 50ml 的 汤A 和 50ml 的 汤B 。
提供 25ml 的 汤A 和 75ml 的 汤B 。
当我们把汤分配给某人之后，汤就没有了。每个回合，我们将从四种概率同为 0.25 的操作中进行分配选择。如果汤的剩余量不足以完成某次操作，我们将尽可能分配。当两种类型的汤都分配完时，停止操作。

注意不存在先分配 100 ml 汤B 的操作。

需要返回的值：汤A先分配完的概率 +汤A和汤B同时分配完的概率 / 2。返回值在正确答案10-5的范围内将被认为是正确的。



示例 1:

输入: n = 50
输出: 0.62500
解释:如果我们选择前两个操作，A 首先将变为空。
对于第三个操作，A 和 B 会同时变为空。
对于第四个操作，B 首先将变为空。
所以 A 变为空的总概率加上 A 和 B 同时变为空的概率的一半是 0.25 *(1 + 1 + 0.5 + 0)= 0.625。
示例 2:

输入: n = 100
输出: 0.71875


提示:

0 <= n <= 109​​​​​​​

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/soup-servings
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    double soupServings(int n) {
        int k = (n + 24) / 25;
        if (n > 4600) return 1;
        vector<vector<double>> dp(k + 1, vector<double>(k + 1));
        int plan[4][2] = {{4, 0}, {3, 1}, {2, 2}, {1, 3}};
        function<double(int, int)> dfs = [&](int ares, int bres) -> double {
            if (ares <= 0 || bres <= 0) {
                if (ares <= 0 && bres <= 0) return 0.5;
                else if (ares <= 0) return 1.0;
                else return 0;
            }
            if (dp[ares][bres] > 0) {
                return dp[ares][bres];
            }
            double sum = 0;
            for (int i = 0; i < 4; i++) {
                double val = dfs(ares - plan[i][0], bres - plan[i][1]) * 0.25;
                sum += val;
            }
            dp[ares][bres] = sum;
            return sum;
        };

        return dfs(k, k);;
    }
};