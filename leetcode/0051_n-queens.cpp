#include "common.h"

/*
按照国际象棋的规则，皇后可以攻击与之处在同一行或同一列或同一斜线上的棋子。
n 皇后问题 研究的是如何将 n 个皇后放置在 n×n 的棋盘上，并且使皇后彼此之间不能相互攻击。
给你一个整数 n ，返回所有不同的 n 皇后问题 的解决方案。
每一种解法包含一个不同的 n 皇后问题 的棋子放置方案，该方案中 'Q' 和 '.' 分别代表了皇后和空位。

示例 1：
输入：n = 4
输出：[[".Q..","...Q","Q...","..Q."],["..Q.","Q...","...Q",".Q.."]]
解释：如上图所示，4 皇后问题存在两个不同的解法。
示例 2：
输入：n = 1
输出：[["Q"]]

提示：
1 <= n <= 9

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/n-queens
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution
{
public:
    vector<vector<string>> solveNQueens(int n)
    {
        vector<vector<string>> res;
        vector<string> ans(n, string(n, '.'));
        unordered_set<int> rows, cols, fslash, bslash;
        function<void(int)> dfs = [&](int depth) {
            if (depth == n) {
                res.push_back(ans);
            }
            for(int i = 0; i < n; i++) {
                int x = depth;
                int y = i;
                if (rows.count(x) || cols.count(y) || fslash.count(y - x) || bslash.count(y + x)) continue;
                rows.insert(x), cols.insert(y), fslash.insert(y - x), bslash.insert(y + x);
                ans[x][y] = 'Q';
                dfs(depth + 1);
                ans[x][y] = '.';
                rows.erase(x), cols.erase(y), fslash.erase(y - x), bslash.erase(y + x); 
            }
        };
        dfs(0);
        return res;
    }
};