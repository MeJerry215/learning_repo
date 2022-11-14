#include "common.h"

/*
给你一个 m 行 n 列的矩阵 matrix ，请按照 顺时针螺旋顺序 ，返回矩阵中的所有元素。

示例 1：
输入：matrix = [[1,2,3],[4,5,6],[7,8,9]]
输出：[1,2,3,6,9,8,7,4,5]
示例 2：
输入：matrix = [[1,2,3,4],[5,6,7,8],[9,10,11,12]]
输出：[1,2,3,4,8,12,11,10,9,5,6,7]

提示：
m == matrix.length
n == matrix[i].length
1 <= m, n <= 10
-100 <= matrix[i][j] <= 100

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/spiral-matrix
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int directions[5] = {0, 1, 0, -1, 0};
        int x = 0, y = 0;
        int n = matrix.size(), m = matrix[0].size();
        int tot = m * n;
        int d = 0, step = m;
        vector<int> res;
        for(int i = 0; i < tot; i++) {
            res.push_back(matrix[x][y]);
            step --;
            if (step == 0) {
                swap(m, n);
                m--;
                step = m;
                d = (d + 1) % 4;
            }
            x += directions[d];
            y += directions[d + 1];
        }
        return res;
    }
};

void TestSolution() {
    Solution s;
    vector<vector<int>> matrix = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10,11,12}
    };
    auto res = s.spiralOrder(matrix);
    print_vec(res);
}

// 1   2   3   4   5
// 16  17  18  19  6
// 15              7
// 14              8
// 13  12  11  10  9