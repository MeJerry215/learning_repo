#include "common.h"

/*
给你两个字符串 s 和 goal ，只要我们可以通过交换 s 中的两个字母得到与 goal 相等的结果，就返回 true ；否则返回 false 。
交换字母的定义是：取两个下标 i 和 j （下标从 0 开始）且满足 i != j ，接着交换 s[i] 和 s[j] 处的字符。
例如，在 "abcd" 中交换下标 0 和下标 2 的元素可以生成 "cbad" 。
 

示例 1：
输入：s = "ab", goal = "ba"
输出：true
解释：你可以交换 s[0] = 'a' 和 s[1] = 'b' 生成 "ba"，此时 s 和 goal 相等。
示例 2：
输入：s = "ab", goal = "ab"
输出：false
解释：你只能交换 s[0] = 'a' 和 s[1] = 'b' 生成 "ba"，此时 s 和 goal 不相等。
示例 3：
输入：s = "aa", goal = "aa"
输出：true
解释：你可以交换 s[0] = 'a' 和 s[1] = 'a' 生成 "aa"，此时 s 和 goal 相等。
 

提示：
1 <= s.length, goal.length <= 2 * 104
s 和 goal 由小写英文字母组成

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/buddy-strings
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution {
public:
    bool buddyStrings(string s, string goal) {
        if(s.size() != goal.size()) return false;
        int a = -1, b = -1, n = s.size(), c[26] = {0}, ds = 0;
        for(int i = 0; i < n; ++i){
            if(++c[s[i]-'a'] == 2) ds = true;
            if(s[i] != goal[i]){
                if(b == -1) a = s[i], b = goal[i];
                else if(a != goal[i] || b != s[i]) return false;
                // 至少2次不同，导致a=-1, 这个时候后续的第3次不同结果不会是对的了
                else a = -1;
            }
        }
        return a == -1 && (b != -1 || ds);
    }
};

void TestSolution() {
    string s = "ab";
    string goal = "ba";
    Solution solution;
    cout << solution.buddyStrings(s, goal) << endl;
    s = "ab", goal = "ab";
    cout << solution.buddyStrings(s, goal) << endl;
    s = "aa", goal = "aa";
    cout << solution.buddyStrings(s, goal) << endl;
}