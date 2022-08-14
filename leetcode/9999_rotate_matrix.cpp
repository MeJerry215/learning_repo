#include "common.h"

class Solution
{
public:
    void rotate(vector<vector<int>> &matrix)
    {
        int M = matrix.size();
        int m = M / 2;
        int n = (M + 1) / 2;
        for (int i = 0; i < M / 2; i++)
        {
            for (int j = 0; j < (M + 1) / 2; j++)
            {
                int tmp = matrix[i][j];
                // i, j 0, 1
                // 3, 0 -> 0, 1
                matrix[i][j] = matrix[M - j - 1][i];
                // 4, 3 -> 3, 0
                matrix[M - j - 1][i] = matrix[M - 1 - i][M - 1 - j];
                // 1, 4 -> 4, 3
                matrix[M - 1 - i][M - 1 - j] = matrix[j][M - 1 - i];
                // 0, 1 -> 1, 4
                matrix[j][M - 1 - i] = tmp;
            }
        }
    }
};

void TestSolution()
{
    vector<vector<int>> matrix = {
        {5, 1, 9, 11},
        {2, 4, 8, 10},
        {13, 3, 6, 7},
        {15, 14, 12, 16}};
    Solution s;
    s.rotate(matrix);
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix.size(); j++)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}