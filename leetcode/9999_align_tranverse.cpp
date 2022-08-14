#include "common.h"

class Solution
{
public:
    vector<int> findDiagonalOrder(vector<vector<int>> &matrix)
    {
        vector<int> ans;
        if (matrix.empty())
            return ans;
        int row = matrix.size(), col = matrix[0].size(), c = 0, r = 0;
        for (int i = 0; i < row + col - 1; i++)
        {
            if (i % 2)
            {
                c = (i < col) ? i : col - 1;
                r = i - c;
                while (c >= 0 && r < row)
                    ans.push_back(matrix[r++][c--]);
            }
            else
            {
                r = (i < row) ? i : row - 1;
                c = i - r;
                while (c < col && r >= 0)
                    ans.push_back(matrix[r--][c++]);
            }
        }
        return ans;
    }
};

void TestSolution()
{
    vector<vector<int>> mat = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Solution s;
    print_mat(mat);
    auto ans = s.findDiagonalOrder(mat);
    print_vec(ans);
    mat = {{1, 2}, {3, 4}};
    ans = s.findDiagonalOrder(mat);
    print_vec(ans);
}