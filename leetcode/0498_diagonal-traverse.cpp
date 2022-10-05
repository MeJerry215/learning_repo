#include "common.h"

/*
给你一个大小为 m x n 的矩阵 mat ，请以对角线遍历的顺序，用一个数组返回这个矩阵中的所有元素。

示例 1：
输入：mat = [[1,2,3],[4,5,6],[7,8,9]]
输出：[1,2,4,7,5,3,6,8,9]
示例 2：
输入：mat = [[1,2],[3,4]]
输出：[1,2,3,4]

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/diagonal-traverse
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

/*
    1       2       3       0(0, 0)  1(0, 1)   (0, 2)
    4       5       6        (1, 0)   (1, 1)   3(1, 2)
    7       8       9       2(2, 0)   (2, 1)   (2, 2)
    10     11      12       (3, 0)   4(3, 1)   5(3, 2) 




*/


class Solution {
public:
    vector<int> findDiagonalOrder(vector<vector<int>> &matrix) {
        vector<int> ans;
        int m = matrix.size();
        int n = matrix[0].size();
        int k = m + n - 1;
        for(int i = 0; i < k; i++) {
            if (i % 2) {
                int col = i < n ? i : n - 1;
                int row = i - col;
                while (col >= 0 && row < m)
                    ans.push_back(matrix[row++][col--]);
            } else {
                int row = i < m ? i : m - 1;
                int col = i - row;
                while (col < n && row >= 0)
                    ans.push_back(matrix[row--][col++]);
            }
        }
        return ans;
    }
};


