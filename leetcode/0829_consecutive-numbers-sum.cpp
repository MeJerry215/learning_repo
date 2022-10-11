#include "common.h"

/*
给定一个正整数 n，返回 连续正整数满足所有数字之和为 n 的组数 。



示例 1:

输入: n = 5
输出: 2
解释: 5 = 2 + 3，共有两组连续整数([5],[2,3])求和后为 5。
示例 2:

输入: n = 9
输出: 3
解释: 9 = 4 + 5 = 2 + 3 + 4
示例 3:

输入: n = 15
输出: 4
解释: 15 = 8 + 7 = 4 + 5 + 6 = 1 + 2 + 3 + 4 + 5


提示:

1 <= n <= 109​​​​​​​

*/
class Solution
{
public:
    int consecutiveNumbersSum(int N)
    {
        int res = 0;
        for (int i = 1; N > 0; N -= i++)
            res += (N % i == 0);
        return res;
    }

    int consecutiveNumbersSum_v1(int n)
    {
        int res = 0;
        int sum = 0;
        int low = 1, high = 1;
        for (; high <= n; high++)
        {
            sum += high;
            if (sum == n)
                res++;
            while (sum >= n && low < high)
            {
                sum -= low++;
                if (sum == n)
                    res++;
            }
        }
        return res;
    }
};
