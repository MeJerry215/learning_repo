#include "common.h"

/*
给你一个字符串 s 、一个字符串 t 。返回 s 中涵盖 t 所有字符的最小子串。如果 s 中不存在涵盖 t 所有字符的子串，则返回空字符串 "" 。

注意：
对于 t 中重复字符，我们寻找的子字符串中该字符数量必须不少于 t 中该字符数量。
如果 s 中存在这样的子串，我们保证它是唯一的答案。

示例 1：
输入：s = "ADOBECODEBANC", t = "ABC"
输出："BANC"
示例 2：
输入：s = "a", t = "a"
输出："a"
示例 3:
输入: s = "a", t = "aa"
输出: ""
解释: t 中两个字符 'a' 均应包含在 s 的子串中，
因此没有符合条件的子字符串，返回空字符串。

提示：
1 <= s.length, t.length <= 105
s 和 t 由英文字母组成
 

进阶：你能设计一个在 o(n) 时间内解决此问题的算法吗？

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/minimum-window-substring
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution
{
public:
    /*
    滑动窗口:
    当前数组不满足t中元素计数的时候 右移 扩张
    当满足t中元素时，收缩。无论在收缩还是扩张阶段 都不统计无关字符, 只统计有效字符数
    扩张阶段，只统计在有效范围内的字符数。
    收缩节点同理，但是要收缩到不在导致不满足t元素计数
    */

    string minWindow(string s, string t) {
        unordered_map<char, int> char_cnt;
        for (int i = 0; i < t.size(); i++) {
            char_cnt[t[i]]++;
        }
        int n = t.size();
        int k = 0;
        int i = 0, j = 0;
        unordered_map<char, int> chars;
        string res = "";
        // 主循环不断扩张
        while (i < s.size()) {
            // 只统计有效字符数目
            if (char_cnt.count(s[i])) {
                chars[s[i]]++;
                // 有效字符必须小于t字符中对应计数才增加
                if (chars[s[i]] <= char_cnt[s[i]])
                    k++;
            }
            // 当满足字符计数时，开始收缩， 收缩到不满足t，其前一个位置就是有效位置。
            if (k == n) {
                while (j <= i) {
                    if (char_cnt.count(s[j])) {
                        chars[s[j]]--;
                        if (chars[s[j]] < char_cnt[s[j]]) {
                            j++;
                            k--;
                            break;
                        }
                    }
                    j++;
                }
                if (res.empty() ||res.size() > (i - j + 2)) {
                    res = s.substr(j - 1, i - j + 2);
                }
            }
            i++;
        }
        return res;
    }
};

void TestSolution() {
    Solution s;
    string s1 = "ab";
    string s2 = "b";
    cout << s.minWindow(s1, s2) << endl;
}