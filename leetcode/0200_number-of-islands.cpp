#include "common.h"

/*
给你一个由 '1'（陆地）和 '0'（水）组成的的二维网格，请你计算网格中岛屿的数量。
岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成。
此外，你可以假设该网格的四条边均被水包围。

示例 1：
输入：grid = [
  ["1","1","1","1","0"],
  ["1","1","0","1","0"],
  ["1","1","0","0","0"],
  ["0","0","0","0","0"]
]
输出：1
示例 2：

输入：grid = [
  ["1","1","0","0","0"],
  ["1","1","0","0","0"],
  ["0","0","1","0","0"],
  ["0","0","0","1","1"]
]
输出：3
 

提示：

m == grid.length
n == grid[i].length
1 <= m, n <= 300
grid[i][j] 的值为 '0' 或 '1'

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/number-of-islands
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        /*
        求岛屿数量，就是求从某个index开始附近的1的数量，这里可以优化visit数组，grid置0。
        可以用bfs或者是dfs求解，但是我这里想要联系的是并查集。
        并查集的概念 init, find, union。如果用上并查集效率上可能并
        */
        int m = grid.size();
        int n = grid[0].size();
        vector<int> fa(m * n, 0);
        iota(fa.begin(), fa.end(), 0);
        function<int(int)> fa_find = [&](int index) {
            if (fa[index] == index) return index;
            return fa[index] = fa_find(fa[index]);
        };
        function<void(int, int)> fa_union = [&](int index, int fa_index) {
            int a = fa_find(index);
            int b = fa_find(fa_index);
            if (a == b) return;
            fa[fa_index] = a;
        };

        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                if (grid[i][j] == '1') {
                    // grid[i][j] = '0';
                    int index = i * n + j;
                    // if (i - 1 >= 0 && grid[i - 1][j] == '1')  fa_union(index, index - n);
                    if (i + 1 < m && grid[i + 1][j] == '1') fa_union(index, index + n);
                    // if (j - 1 >= 0 && grid[i][j - 1] == '1') fa_union(index, index - 1);
                    if (j + 1 < n && grid[i][j + 1] == '1') fa_union(index, index + 1);
                }
            }
        }

        unordered_set<int> qqq;
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                if (fa[i * n + j] == i *n + j && grid[i][j] == '1') {
                    qqq.insert(fa[i * n + j]);
                }
            }
        }
        return qqq.size();
    }
};
// 0       0       2       3       4
// 0       0       7       8       9 
// 10      11      12      13      14
// 15      16      17      18      18

void TestSolution() {
    Solution s;
    vector<vector<char>> island = {
        {'1','1','0','0','0'},
        {'1','1','0','0','0'},
        {'0','0','1','0','0'},
        {'0','0','0','1','1'}
    };

    cout << s.numIslands(island) << endl;
}