#include "common.h"

/*
给定一个数组，它的第 i 个元素是一支给定的股票在第 i 天的价格。
设计一个算法来计算你所能获取的最大利润。你最多可以完成 两笔 交易。
注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。

示例 1:
输入：prices = [3,3,5,0,0,3,1,4]
输出：6
解释：在第 4 天（股票价格 = 0）的时候买入，在第 6 天（股票价格 = 3）的时候卖出，这笔交易所能获得利润 = 3-0 = 3 。
     随后，在第 7 天（股票价格 = 1）的时候买入，在第 8 天 （股票价格 = 4）的时候卖出，这笔交易所能获得利润 = 4-1 = 3 。
示例 2：
输入：prices = [1,2,3,4,5]
输出：4
解释：在第 1 天（股票价格 = 1）的时候买入，在第 5 天 （股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5-1 = 4 。  
     注意你不能在第 1 天和第 2 天接连购买股票，之后再将它们卖出。  
     因为这样属于同时参与了多笔交易，你必须在再次购买前出售掉之前的股票。
示例 3：
输入：prices = [7,6,4,3,1]
输出：0
解释：在这个情况下, 没有交易完成, 所以最大利润为 0。
示例 4：
输入：prices = [1]
输出：0

提示：
1 <= prices.length <= 105
0 <= prices[i] <= 105
通过次数221,593提交次数383,805

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-iii
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

/*
dp[i][k][0] = max(dp[i - 1][k][0], dp[i - 1][k] + prices[i])
dp[i][k][1] = max(dp[i - 1][k][1], dp[i][k - 1][0] - prices[i]);
*/
class Solution
{
public:
    int maxProfit_v1(vector<int> &prices)
    {
        int n = prices.size();
        vector<vector<vector<int>>> profits(n, vector<vector<int>>(3, vector<int>(2, 0)));
        // int profits[n][3][2] = {};
        profits[0][0][0] = 0;
        profits[0][1][1] = -prices[0];
        profits[0][2][1] = -prices[0];
        for (int i = 1; i < n; i++)
        {
            for (int j = 1; j <= 2; j++)
            {
                profits[i][j][0] = max(profits[i - 1][j][0], profits[i - 1][j][1] + prices[i]);
                profits[i][j][1] = max(profits[i - 1][j][1], profits[i][j - 1][0] - prices[i]);
            }
        }
        return max(profits[n - 1][1][0], profits[n - 1][2][0]);
    }

    int maxProfit(vector<int> &prices)
    {
        int p = 0, q = -prices[0], s = 0, t = -prices[0];
        for (int i = 1; i < prices.size(); i++)
        {
            p = max(p, q + prices[i]);
            q = max(q, -prices[i]);
            s = max(s, t + prices[i]);
            t = max(t, p - prices[i]);
        }
        return s;
    }
};