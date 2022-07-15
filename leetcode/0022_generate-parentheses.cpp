#include "common.h"

/*
数字 n 代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且 有效的 括号组合。

示例 1：
输入：n = 3
输出：["((()))","(()())","(())()","()(())","()()()"]
示例 2：
输入：n = 1
输出：["()"]

提示：
1 <= n <= 8
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/generate-parentheses
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution {
public:
    void dfs_generate(vector<string> &ans, string& parent, int l, int r) {
        if (l == 0 && r == 0) {
            ans.push_back(parent);
            return;
        }
        if (l > 0) {
            parent.push_back('(');
            dfs_generate(ans, parent, l - 1, r);
            parent.pop_back();
        }
        if (l < r) {
            parent.push_back(')');
            dfs_generate(ans, parent, l, r - 1);
            parent.pop_back();
        }
    }

    vector<string> generateParenthesis(int n) {
        vector<string> ans;
        string parent = "";
        dfs_generate(ans, parent, n, n);
        return ans;
    }
};

void TestSolution() {
    Solution s;
    auto ans = s.generateParenthesis(0);
    print_vec(ans);
}