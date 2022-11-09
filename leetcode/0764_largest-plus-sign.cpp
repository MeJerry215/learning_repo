#include "common.h"

/*
在一个 n x n 的矩阵grid中，除了在数组mines中给出的元素为0，其他每个元素都为1。mines[i] = [xi, yi]表示grid[xi][yi] == 0
返回 grid 中包含1的最大的 轴对齐 加号标志的阶数 。如果未找到加号标志，则返回 0 。
一个k阶由1组成的 “轴对称”加号标志 具有中心网格grid[r][c] == 1，以及4个从中心向上、向下、向左、向右延伸，长度为k-1，由1组成的臂。注意，只有加号标志的所有网格要求为 1 ，别的网格可能为 0 也可能为 1 。

示例 1：



输入: n = 5, mines = [[4, 2]]
输出: 2
解释: 在上面的网格中，最大加号标志的阶只能是2。一个标志已在图中标出。
示例 2：



输入: n = 1, mines = [[0, 0]]
输出: 0
解释: 没有加号标志，返回 0 。


提示：

1 <= n <= 500
1 <= mines.length <= 5000
0 <= xi, yi< n
每一对(xi, yi)都 不重复​​​​​​​
通过次数6,089提交次数12,185

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/largest-plus-sign
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution
{
public:
    int orderOfLargestPlusSign(int n, vector<vector<int>> &mines)
    {
        vector<vector<int>> grid(n, vector<int>(n, n));
        unordered_set<int> banned;
        for (int i = 0; i < mines.size(); i++)
        {
            banned.emplace(mines[i][0] * n + mines[i][1]);
        }
        int res = 0;
        for (int i = 0; i < n; i++)
        {
            int count = 0;
            for (int j = 0; j < n; j++)
            {
                if (banned.count(i * n + j))
                {
                    count = 0;
                }
                else
                {
                    count++;
                }
                grid[i][j] = min(grid[i][j], count);
            }
            count = 0;
            for (int j = n - 1; j >= 0; j--)
            {
                if (banned.count(i * n + j))
                {
                    count = 0;
                }
                else
                {
                    count++;
                }
                grid[i][j] = min(grid[i][j], count);
            }
        }
        print_mat(grid);
        for (int i = 0; i < n; i++)
        {
            int count = 0;
            for (int j = 0; j < n; j++)
            {
                if (banned.count(j * n + i))
                {
                    count = 0;
                }
                else
                {
                    count++;
                }
                grid[j][i] = min(grid[j][i], count);
            }
            count = 0;
            for (int j = n - 1; j >= 0; j--)
            {
                if (banned.count(j * n + i))
                {
                    count = 0;
                }
                else
                {
                    count++;
                }
                grid[j][i] = min(grid[j][i], count);
                res = max(res, grid[j][i]);
            }
        }
        return res;
    }
};


void TestSolution() {
    Solution s;
    vector<vector<int>> mines = {
        {0, 1},
    };
    s.orderOfLargestPlusSign(3, mines);
}