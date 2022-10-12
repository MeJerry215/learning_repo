#include "common.h"

/*
爱丽丝参与一个大致基于纸牌游戏 “21点” 规则的游戏，描述如下：
爱丽丝以 0 分开始，并在她的得分少于 k 分时抽取数字。 抽取时，她从 [1, maxPts] 的范围中随机获得一个整数作为分数进行累计，其中 maxPts 是一个整数。 每次抽取都是独立的，其结果具有相同的概率。
当爱丽丝获得 k 分 或更多分 时，她就停止抽取数字。
爱丽丝的分数不超过 n 的概率是多少？
与实际答案误差不超过 10^-5 的答案将被视为正确答案。
 
示例 1：
输入：n = 10, k = 1, maxPts = 10
输出：1.00000
解释：爱丽丝得到一张牌，然后停止。
示例 2：
输入：n = 6, k = 1, maxPts = 10
输出：0.60000
解释：爱丽丝得到一张牌，然后停止。 在 10 种可能性中的 6 种情况下，她的得分不超过 6 分。
示例 3：
输入：n = 21, k = 17, maxPts = 10
输出：0.73278

提示：
0 <= k <= n <= 104
1 <= maxPts <= 104
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/new-21-game
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/


/*
这道题目没有做出来，以为是从正向取算alice获胜的概率，没有读懂题目。
alice 可以一直抽牌，抽牌的点数为[1, maxPts]之间,  抽牌总和没有达到k的时候就需要继续抽牌，直到超过k
所以alice 根据手上的牌 来看 [k - maxPts, k - 1]。所以alice能够抽到的最大牌的范围就确定为 k - 1 + maxPts
所以下面的例子为例 : m = 21, k =17, maxptrs = 10
              
              |                                          k - 1 + maxPts
    15  16 |  17  18  19  20  21 |  22  23  24  25  26 
               1   1   1   1   1    0   0   0  0    0

    根据以上是事实 alice 抽取到[17 - 21]之间 获胜， [22, 26]之间失败

    此时计算 16的
*/

class Solution {
public:
    double new21Game(int n, int k, int maxPts) {
        if (k == 0) {
            return 1.0;
        }
        vector<double> dp(k + maxPts);
        for (int i = k; i <= n && i < k + maxPts; i++) {
            dp[i] = 1.0;
        }
        dp[k - 1] = 1.0 * min(n - k + 1, maxPts) / maxPts;
        for (int i = k - 2; i >= 0; i--) {
            dp[i] = dp[i + 1] - (dp[i + maxPts + 1] - dp[i + 1]) / maxPts;
        }
        return dp[0];
    }
};



void TestSolution() {
    Solution s;
    s.new21Game(21, 17, 10);
}