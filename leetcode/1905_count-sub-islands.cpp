#include "common.h"

class Solution {
public:
    int countSubIslands(vector<vector<int>>& grid1, vector<vector<int>>& grid2) {
        int color = 2;
        int m = grid1.size();
        int n = grid1[0].size();
        int directions[5] = {0, 1, 0, -1, 0};
        function<void(int, int, int)> paint = [&](int x, int y, int color) {
            grid1[x][y] = color;
            for(int i = 0; i < 4; i++) {
                int p = x + directions[i];
                int q = y + directions[i + 1];
                if (p < 0 || q < 0 || p >= m || q >= n || grid1[p][q] != 1) continue;
                paint(p, q, color);
            }
        };
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                if (grid1[i][j] == 1) {
                    paint(i, j, color++);
                }
            }
        }
        unordered_set<int> excludes;
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                if (grid1[i][j] > 0 && grid2[i][j] == 0) {
                    excludes.insert(grid1[i][j]);
                }
            }
        }
        return color - excludes.size() - 1;
    }

    int countSubIslands(vector<vector<int>>& grid1, vector<vector<int>>& grid2) {
        int m = grid1.size();
        int n = grid1[0].size();
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                grid2[i][j] += grid1[i][j];
            }
        }
        int res = 0;
        int directions[5] = {0, 1, 0, -1, 0};
        function<void(int, int, int)> paint = [&](int x, int y, int color) {
            grid2[x][y] = color;
            for(int i = 0; i < 4; i++) {
                int p = x + directions[i];
                int q = y + directions[i + 1];
                if (p < 0 || q < 0 || p >= m || q >= n || grid2[p][q] != 2) continue;
                paint(p, q, color);
            }
        };

        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                if (grid2[i][j] == 2) {
                    paint(i, j, 0);
                }
            }
        }
    }
};