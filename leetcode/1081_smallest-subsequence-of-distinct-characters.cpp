#include "common.h"

/*
返回 s 字典序最小的子序列，该子序列包含 s 的所有不同字符，且只包含一次。
注意：该题与 316 https://leetcode.com/problems/remove-duplicate-letters/ 相同

示例 1：
输入：s = "bcabc"
输出："abc"
示例 2：
输入：s = "cbacdcbc"
输出："acdb"

提示：
1 <= s.length <= 1000
s 由小写英文字母组成
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/smallest-subsequence-of-distinct-characters
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

/*
b c a b c
3 3 3 2 1
*/

class Solution {
public:
    string smallestSubsequence(string s) {
        unordered_set<char> chars;
        vector<int> char_cnts(s.size(), 0);
        for(int i = s.size() - 1; i >= 0; i++) {
            chars.insert(s[i]);
            char_cnts[i] = chars.size();
        }
        string res;
        chars.clear();
        int i = 0;
        while(i < s.size()) {
            char min_char = s[i];
            while(i < s.size() && char_cnts[i + 1] == char_cnts[i]) {
                i++;
                if (!chars.count(s[i])) {
                    min_char = min(min_char, s[i]);
                }
            }
            res.push_back(min_char);
            chars.insert(min_char);
            i++;
        }
        return res;
    }
};
