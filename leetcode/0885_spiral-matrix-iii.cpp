
#include "common.h"
/*
在 R 行 C 列的矩阵上，我们从 (r0, c0) 面朝东面开始
这里，网格的西北角位于第一行第一列，网格的东南角位于最后一行最后一列。
现在，我们以顺时针按螺旋状行走，访问此网格中的每个位置。
每当我们移动到网格的边界之外时，我们会继续在网格之外行走（但稍后可能会返回到网格边界）。
最终，我们到过网格的所有 R * C 个空间。
按照访问顺序返回表示网格位置的坐标列表。

示例 1：

输入：R = 1, C = 4, r0 = 0, c0 = 0
输出：[[0,0],[0,1],[0,2],[0,3]]

示例 2：

输入：R = 5, C = 6, r0 = 1, c0 = 4
输出：[[1,4],[1,5],[2,5],[2,4],[2,3],[1,3],[0,3],[0,4],[0,5],[3,5],[3,4],[3,3],[3,2],[2,2],[1,2],[0,2],[4,5],[4,4],[4,3],[4,2],[4,1],[3,1],[2,1],[1,1],[0,1],[4,0],[3,0],[2,0],[1,0],[0,0]]

提示：
1 <= R <= 100
1 <= C <= 100
0 <= r0 < R
0 <= c0 < C
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/spiral-matrix-iii
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution
{
public:
    /*
   N
 W   E
   S
                5
 -1          21 →22 →23 →24 →25 →26
     0   1   2↑  3   4   5       ↓5
 0           20  7 → 8 → 9  →10  27
             ↑   ↑ 3    1    ↓3  ↓
 1           19  6   1 → 2   11  28
             ↑  2↑       ↓ 1 ↓   ↓
 2           18  5 ← 4 ← 3   12  29
           4 ↑         2     ↓   ↓
 3           17← 16← 15← 14← 13  30
                            4    ↓
 4                       33← 32← 31
 */

    vector<int> directions = {
        0, 1, 0, -1, 0};

    vector<vector<int>> spiralMatrixIII(int rows, int cols, int rStart, int cStart)
    {
        int n_elements = rows * cols;
        bool towards_east = true;
        bool towards_south = true;
        vector<vector<int>> res;
        int step_limit = 1;
        int step = 0;
        int dir = 0; // 0 east 1 south 2 west 3 north
        while (res.size() < n_elements)
        {
            if (rStart >= 0 && rStart < rows && cStart >= 0 && cStart < cols)
            {
                res.push_back({rStart, cStart});
            }
            // walk a step
            rStart += directions[dir];
            cStart += directions[dir + 1];
            step++;
            /* now we need to change a direction and reset step count */
            if (step == step_limit)
            {
                step = 0;
                dir += 1;
                dir %= 4;
                /* only direction change to east or west increase step limit */
                if (dir == 0 || dir == 2)
                    step_limit++;
            }
        }
        return res;
    }
};