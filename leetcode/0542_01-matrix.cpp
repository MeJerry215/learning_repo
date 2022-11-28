#include "common.h"

class Solution {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        int m = mat.size();
        int n = mat[0].size();
        vector<vector<int>> res(m, vector<int>(n, INT_MAX));
        queue<pair<int, int>> tmp;
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                if (mat[i][j] == 0) {
                    tmp.push({i * n + j, 0});
                }
            }
        }
        int directions[5] = {0, 1, 0, -1, 0};
        while(!tmp.empty()) {
            int k = tmp.size();
            for(int i = 0; i < k; i++) {
                pair<int,int> state = tmp.front();
                tmp.pop();
                int x = state.first / n;
                int y = state.first % n;
                int step = state.second;
                if (res[x][y] != INT_MAX) continue;
                res[x][y] = step;
                for(int j = 0; j < 4; j ++) {
                    int p = x + directions[j];
                    int q = y + directions[j + 1];
                    if (p >= m || q >= m || p < 0 || q < 0) continue;
                    tmp.push({p * n + q, step + 1});
                }
            }
        }
        return res;
    }
};

void TestSolution() {
    Solution s;
    vector<vector<int>> mat = {
        {0, 0, 0},
        {0, 1, 0},
        {1, 1, 1}};
    auto res = s.updateMatrix(mat);
    print_mat(res);
}