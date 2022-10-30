#include "common.h"

/*
给定一个字符串 s ，通过将字符串 s 中的每个字母转变大小写，我们可以获得一个新的字符串。
返回 所有可能得到的字符串集合 。以 任意顺序 返回输出。

示例 1：

输入：s = "a1b2"
输出：["a1b2", "a1B2", "A1b2", "A1B2"]
示例 2:

输入: s = "3z4"
输出: ["3z4","3Z4"]
 

提示:

1 <= s.length <= 12
s 由小写英文字母、大写英文字母和数字组成

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/letter-case-permutation
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    vector<string> letterCasePermutation(string s) {
        vector<string> res;
        function<void(int)> dfs = [&](int idx) {
            if (idx == s.size()) {
                res.push_back(s);
                return;
            }
            if (isalpha(s[idx])) {
                dfs(idx + 1);
                s[idx] = s[idx] ^ 32;
                dfs(idx + 1);
            } else {
                dfs(idx+1);
            }
        };
        dfs(0);
        return res;
    }
};