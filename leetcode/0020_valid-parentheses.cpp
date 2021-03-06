#include "common.h"



/*
给定一个只包括 '('，')'，'{'，'}'，'['，']' 的字符串 s ，判断字符串是否有效。
有效字符串需满足：

左括号必须用相同类型的右括号闭合。
左括号必须以正确的顺序闭合。

示例 1：
输入：s = "()"
输出：true
示例 2：
输入：s = "()[]{}"
输出：true
示例 3：
输入：s = "(]"
输出：false
示例 4：
输入：s = "([)]"
输出：false
示例 5：
输入：s = "{[]}"
输出：true
提示：

1 <= s.length <= 104
s 仅由括号 '()[]{}' 组成

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/valid-parentheses
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/



class Solution {
public:
    bool isValid(string s) {
            vector<int> brack_stack;
            unordered_map<char, char> maps = {
                {']', '['},
                {')', '('},
                {'}', '{'}
            };
            for(int i = 0; i < s.size(); i++) {
                if (s[i] == '[' || s[i] == '{' || s[i] == '(') {
                    brack_stack.push_back(s[i]);
                } else if (s[i] == ']' || s[i] == '}' || s[i] == ')') {
                    if (brack_stack.empty() || brack_stack.back() != maps[s[i]]) return false;
                    brack_stack.pop_back();
                }
            }
            return brack_stack.empty();
    }
};