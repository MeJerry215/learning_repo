#include "common.h"

class NumMatrix {
public:
    vector<vector<int>> sum_matrix;
    int m, n;

    NumMatrix(vector<vector<int>>& matrix) {
        m = matrix.size() + 1;
        n = matrix[0].size() + 1;
        for(int i = 0; i < m; i++) {
            sum_matrix.emplace_back(vector<int>(n, 0));
        }
        for(int i = 0; i < matrix.size(); i ++) {
            for(int j = 0; j < matrix[0].size(); j++) {
                sum_matrix[i + 1][j + 1] = matrix[i][j] + sum_matrix[i, j + 1] + sum_matrix[i + 1, j] - sum_matrix[i, j];
            }
        }
        print_mat(sum_matrix);

    }
    
    int sumRegion(int row1, int col1, int row2, int col2) {
        return sum_matrix[row2 + 1][col2 + 1] - sum_matrix[row1][col2 + 1] - sum_matrix[row2 + 1][col1] + P[row1][col1];
    }
};

/**
 * Your NumMatrix object will be instantiated and called as such:
 * NumMatrix* obj = new NumMatrix(matrix);
 * int param_1 = obj->sumRegion(row1,col1,row2,col2);
 */