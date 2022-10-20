#include "common.h"



class Solution {
public:
    vector<int> luckyNumbers (vector<vector<int>>& matrix) {
        vector<int> row_min(matrix.size(), INT_MAX);
        vector<int> col_max(matrix[0].size(), 0);
        for(int i = 0; i < matrix.size(); i++) {
            for(int j = 0; j < matrix[0].size(); j++) {
                row_min[i] = min(matrix[i][j], row_min[i]);
                col_max[j] = max(matrix[i][j], col_max[j]);
            }
        }
        vector<int> res;
        for(int i = 0; i < row_min.size(); i++) {
            if (find(col_max.begin(), col_max.end(), row_min[i]) != col_max.end()) {
                res.push_back(row_min[i]);
            }
        }
        return res;
    }
};