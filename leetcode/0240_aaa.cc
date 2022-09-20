#include "common.h"

class Solution
{
public:
    bool searchMatrix(vector<vector<int>> &matrix, int target)
    {
        int m = matrix.size();
        int n = matrix[0].size();
        for (int i = 0; i < matrix.size(); i++)
        {
            auto &row = matrix[i];
            if (row[0] > target)
                return false;
            else if (row[n - 1] < target)
                continue;
            int low = 0, high = n - 1;
            while (low <= high)
            {
                int mid = (low + high) / 2;
                if (matrix[i][mid] == target)
                    return true;
                else if (matrix[i][mid] < target)
                    low = mid + 1;
                else
                    high = mid - 1;
            }
        }
        return false;
    }

    bool searchMatrix_v2(vector<vector<int>> &matrix, int target)
    {
        return searchMatrixHelper(matrix, target, 0, matrix[0].size() - 1, 0, matrix.size() - 1);
    }

    bool searchMatrixHelper(vector<vector<int>> &matrix, int target, int l, int r, int b, int t)
    {
        // 提前结束
        if (l > r || b > t)
            return false;
        if (matrix[b][l] > target || matrix[t][r] < target)
            return false;
        int col_mid = (l + r) / 2;
        int row_mid = (b + t) / 2;
        if (matrix[row_mid][col_mid] == target)
            return true;
        else if (matrix[row_mid][col_mid] > target)
        {
            return searchMatrixHelper(matrix, target, l, col_mid, b, row_mid) ||
                searchMatrixHelper(matrix, target, col_mid + 1, r, b, row_mid) ||
                searchMatrixHelper(matrix, target, l, col_mid, row_mid + 1, t) ;
        }
        else
        {
            return searchMatrixHelper(matrix, target, col_mid + 1, r, b, row_mid) ||
                   searchMatrixHelper(matrix, target, l, col_mid, row_mid + 1, t) ||
                   searchMatrixHelper(matrix, target, col_mid + 1, r, row_mid + 1, t);
        }
        return false;
    }
};
// @lc code=end
// #include <iostream>
