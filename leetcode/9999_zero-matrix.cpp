#include "common.h"

class Solution
{
public:
    void setZeroes(vector<vector<int>> &matrix)
    {
        vector<std::pair<int, int>> indexs;
        int M = matrix.size();
        int N = matrix[0].size();
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (matrix[i][j] == 0)
                {
                    indexs.push_back(make_pair(i, j));
                }
            }
        }
        for (auto &index : indexs)
        {
            int i = index.first;
            for (int j = 0; j < N; j++)
            {
                matrix[i][j] = 0;
            }
            int j = index.second;
            for (int i = 0; i < M; i++)
            {
                matrix[i][j] = 0;
            }
        }
    }
};

void TestSolution()
{
    Solution s;
    vector<vector<int>> matrix = {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}};
    s.setZeroes(matrix);

    matrix = {
        {0, 1, 2, 0},
        {3, 4, 5, 2},
        {1, 3, 1, 5}};
    s.setZeroes(matrix);
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[0].size(); j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}