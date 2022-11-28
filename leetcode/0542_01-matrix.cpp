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
                if (p >= m || q >= n || p < 0 || q < 0 || res[p][q] != INT_MAX) continue;
                tmp.push({p * n + q, step + 1});
            }
        }
        return res;
    }

    vector<vector<int>> updateMatrix(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        // 初始化动态规划的数组，所有的距离值都设置为一个很大的数
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX / 2));
        // 如果 (i, j) 的元素为 0，那么距离为 0
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] == 0) {
                    dist[i][j] = 0;
                }
            }
        }
        // 只有 水平向左移动 和 竖直向上移动，注意动态规划的计算顺序
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i - 1 >= 0) {
                    dist[i][j] = min(dist[i][j], dist[i - 1][j] + 1);
                }
                if (j - 1 >= 0) {
                    dist[i][j] = min(dist[i][j], dist[i][j - 1] + 1);
                }
            }
        }
        // 只有 水平向右移动 和 竖直向下移动，注意动态规划的计算顺序
        for (int i = m - 1; i >= 0; --i) {
            for (int j = n - 1; j >= 0; --j) {
                if (i + 1 < m) {
                    dist[i][j] = min(dist[i][j], dist[i + 1][j] + 1);
                }
                if (j + 1 < n) {
                    dist[i][j] = min(dist[i][j], dist[i][j + 1] + 1);
                }
            }
        }
        return dist;
    }

};

void TestSolution() {
    Solution s;
    vector<vector<int>> mat = {
        {0},
        {0},
        {0}
    };
    auto res = s.updateMatrix(mat);
    print_mat(res);
}