#include "common.h"

class Solution {
public:
    vector<vector<int>> onesMinusZeros(vector<vector<int>>& grid) {
        // row 0 col 1 one
        int m = grid.size(), n = grid[0].size();
        vector<int> row_cnts(m, 0);
        vector<int> col_cnts(n, 0);
        vector<vector<int>> res(m, vector<int>(n, 0));
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                if (grid[i][j]) {
                    row_cnts[i] ++;
                    col_cnts[j] ++;
                }
            }
        }
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                res[i][j] = 2 *  row_cnts[i] +  2 * col_cnts[j] - m - n;
            }
        }

        return res;

    }
};