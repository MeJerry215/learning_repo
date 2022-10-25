#include "common.h"

/*
给你一个大小为 n x n 的二元矩阵 grid ，其中 1 表示陆地，0 表示水域。
岛 是由四面相连的 1 形成的一个最大组，即不会与非组内的任何其他 1 相连。grid 中 恰好存在两座岛 。
你可以将任意数量的 0 变为 1 ，以使两座岛连接起来，变成 一座岛 。
返回必须翻转的 0 的最小数目。

示例 1：
输入：grid = [[0,1],[1,0]]
输出：1
示例 2：
输入：grid = [[0,1,0],[0,0,0],[0,0,1]]
输出：2
示例 3：
输入：grid = [[1,1,1,1,1],[1,0,0,0,1],[1,0,1,0,1],[1,0,0,0,1],[1,1,1,1,1]]
输出：1
提示：
n == grid.length == grid[i].length
2 <= n <= 100
grid[i][j] 为 0 或 1
grid 中恰有两个岛

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/shortest-bridge
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/


class Solution {
public:
    int shortestBridge(vector<vector<int>>& grid) {
        int n = grid.size();
        vector<vector<int>> dirs = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
        vector<pair<int, int>> island;
        queue<pair<int, int>> qu;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    qu.emplace(i, j);
                    grid[i][j] = -1;
                    while (!qu.empty()) {
                        auto [x, y] = qu.front();
                        qu.pop();
                        island.emplace_back(x, y);
                        for (int k = 0; k < 4; k++) {
                            int nx = x + dirs[k][0];
                            int ny = y + dirs[k][1];
                            if (nx >= 0 && ny >= 0 && nx < n && ny < n && grid[nx][ny] == 1) {
                                qu.emplace(nx, ny);
                                grid[nx][ny] = -1;
                            }
                        }
                    }
                    for (auto &&[x, y] : island) {
                        qu.emplace(x, y);
                    }
                    int step = 0;
                    while (!qu.empty()) {
                        int sz = qu.size();
                        for (int i = 0; i < sz; i++) {
                            auto [x, y] = qu.front();
                            qu.pop();
                            for (int k = 0; k < 4; k++) {
                                int nx = x + dirs[k][0];
                                int ny = y + dirs[k][1];
                                if (nx >= 0 && ny >= 0 && nx < n && ny < n) {
                                    if (grid[nx][ny] == 0) {
                                        qu.emplace(nx, ny);
                                        grid[nx][ny] = -1;
                                    } else if (grid[nx][ny] == 1) {
                                        return step;
                                    }
                                }
                            }
                        }
                        step++;
                    }
                }
            }
        }
        return 0;
    }
};


class Solution {
public:
    int directions[5] = {0, 1, 0, -1, 0};

    bool is_valid_index(int n, int x, int y) {
        return x >= 0 && y >= 0 && x < n && y < n;
    }

    int shortestBridge(vector<vector<int>>& grid) {
        int n = grid.size();
        queue<pair<int, int>> edge_nodes;
        for(int i = 0; i < n; i++) {
            bool is_color = false;
            for(int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    is_color = true;
                    queue<pair<int, int>> indexs;
                    indexs.emplace(i, j);
                    while(indexs.size()) {
                        pair<int, int> index = indexs.front();
                        indexs.pop();
                        grid[index.first][index.second] = 2;
                        bool is_edge = false;
                        for(int k = 0; k < 4; k++) {
                            int x = index.first + directions[k];
                            int y = index.second + directions[k + 1];
                            bool valid_flag = is_valid_index(n, x, y);
                            if (valid_flag) {
                                if (grid[x][y] == 1) {
                                    indexs.emplace(x, y);
                                } else if (grid[x][y] == 0) {
                                    is_edge = true;
                                }
                            }
                        }
                        if (is_edge) edge_nodes.push(move(index));
                    }
                    break;
                }
            }
            if (is_color) break;
        }
        int res = 0;
        while(true) {
            res ++;
            int k = edge_nodes.size();
            for(int i = 0; i < k; i++) {
                pair<int, int>& index = edge_nodes.front();
                for(int j = 0; j < 4; j ++) {
                    int x = index.first + directions[j];
                    int y = index.second + directions[j + 1];
                    if (!is_valid_index(n, x, y)) continue;
                    if (grid[x][y] == 1) return res - 1;
                    if (grid[x][y] == 0) {
                        grid[x][y] = 2;
                        edge_nodes.emplace(x, y);
                    }
                }
                edge_nodes.pop();
            }
        }
        return res;
    }
};


void TestSolution() {
    Solution s;
    vector<vector<int>> grid  = {
        {0,1},
        {1,0},
    };
    cout << s.shortestBridge(grid) << endl;
}