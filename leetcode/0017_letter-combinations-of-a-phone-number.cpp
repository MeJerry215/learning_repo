#include "common.h"

/*
给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合。答案可以按 任意顺序 返回。

给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。



 

示例 1：

输入：digits = "23"
输出：["ad","ae","af","bd","be","bf","cd","ce","cf"]
示例 2：

输入：digits = ""
输出：[]
示例 3：

输入：digits = "2"
输出：["a","b","c"]
 

提示：

0 <= digits.length <= 4
digits[i] 是范围 ['2', '9'] 的一个数字。

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/letter-combinations-of-a-phone-number
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/


class Solution {
public:

    void dfs_letter_combinations(vector<string>& ans, string& digits, string& cur) {
        if (cur.size() == digits.size()) {
            ans.push_back(cur);
            return;
        }
        int idx = cur.size();
        int k = digits[idx] == '9' || digits[idx] == '7' ? 4 : 3;
        for (int i = 0; i < k; i++) {
            cur.push_back('a' + i + (digits[idx] - '2') * 3 + (digits[idx] - '0') / 8);
            dfs_letter_combinations(ans, digits, cur);
            cur.pop_back();
        }
    }

    vector<string> letterCombinations(string digits) {
        if (digits.size() == 0) return vector<string>();
        vector<string> ans;
        string cur = "";
        
        dfs_letter_combinations(ans, digits, cur);
        return ans;
    }
};

void TestSolution() {
    Solution s;
    string digits = "23";
    auto ans = s.letterCombinations(digits);
    print_vec(ans);
    digits = "";
    ans = s.letterCombinations(digits);
    print_vec(ans);

}