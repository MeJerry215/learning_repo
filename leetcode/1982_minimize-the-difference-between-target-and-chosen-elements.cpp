#include "common.h"

/*
给你一个大小为 m x n 的整数矩阵 mat 和一个整数 target 。
从矩阵的 每一行 中选择一个整数，你的目标是 最小化 所有选中元素之 和 与目标值 target 的 绝对差 。
返回 最小的绝对差 。
a 和 b 两数字的 绝对差 是 a - b 的绝对值。

示例 1：
输入：mat = [[1,2,3],[4,5,6],[7,8,9]], target = 13
输出：0
解释：一种可能的最优选择方案是：
- 第一行选出 1
- 第二行选出 5
- 第三行选出 7
所选元素的和是 13 ，等于目标值，所以绝对差是 0 。
示例 2：
输入：mat = [[1],[2],[3]], target = 100

提示：

m == mat.length
n == mat[i].length
1 <= m, n <= 70
1 <= mat[i][j] <= 70
1 <= target <= 800
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/minimize-the-difference-between-target-and-chosen-elements
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    /*
    dfs failed
    时间复杂度太高。此种方法每次遍历都存在n种选择，递归栈深度m，则最后的时间复杂度O(n^m), 确实很大
    所以查看提示后可以知道，实际上
    由于数字组合第一层，
    1 , 2, 3
    第二层
    5, 6, 7, 8, 9  只有5种可能空间，原本3*3的可能空间被压缩到5  
    */

    int minimizeTheDifference(vector<vector<int>>& mat, int target) { 
        unordered_set<int> probs{0};
        for(auto &row : mat) {
            unordered_set<int> row_prob;
            int minimal = INT_MAX;
            for(auto num: row) {
                for(int prob: probs) {
                    if (prob + num <= target)
                        row_prob.insert(prob + num);
                    else {

                        minimal = min(minimal, prob + num);
                    }
                }
            }
            if (minimal != INT_MAX) {
                row_prob.insert(minimal);
            }
            probs = move(row_prob);
        }
        int min_diff = INT_MAX;
        for(auto prob: probs) {
            min_diff = min(min_diff, abs(target - prob));
        }
        return min_diff;
    }


    int minimizeTheDifference_v1(vector<vector<int>>& mat, int target) {
        int res = INT_MAX;
        int m = mat.size();
        int n = mat[0].size();
        function<void(int, int)> dfs = [&](int depth, int sum_tot) {
            int diff = sum_tot - target;
            // no need to sum, cause all value are postive,
            if (diff > 0 && diff > res) {
                return;
            }
            if (depth == m) {
                res = min(res, abs(diff));
                return;
            }
            for(int i = 0; i < n; i++) {
                dfs(depth + 1, sum_tot + mat[depth][i]);
            }
        };
        dfs(0, 0);
        return res;
    }
};