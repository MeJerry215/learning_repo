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
    vector<string> generateParenthesis(int n) {
        vector<string> res;
        string str = "";
        function<void(int, int, int)> dfs = [&](int left_l_num, int left_r_num, int score) {
            if (str.size() == 2 * n) res.push_back(str);
            if (left_l_num) {
                str.push_back('(');
                dfs(left_l_num - 1, left_r_num, score - 1);
                str.pop_back();
            }
            if (left_r_num && score < 0) {
                str.push_back(')');
                dfs(left_l_num, left_r_num - 1, score + 1);
                str.pop_back();
            }
        };
        dfs(n, n, 0);
        return res;
    }
};

void TestSolution() {
    Solution s;
    auto ans = s.generateParenthesis(0);
    print_vec(ans);
}