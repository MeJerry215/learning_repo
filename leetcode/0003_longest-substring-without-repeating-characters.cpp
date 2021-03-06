#include "common.h"

/*
给定一个字符串 s ，请你找出其中不含有重复字符的 最长子串 的长度。

示例 1:
输入: s = "abcabcbb"
输出: 3
解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
示例 2:
输入: s = "bbbbb"
输出: 1
解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。
示例 3:
输入: s = "pwwkew"
输出: 3
解释: 因为无重复字符的最长子串是 "wke"，所以其长度为 3。
     请注意，你的答案必须是 子串 的长度，"pwke" 是一个子序列，不是子串。
提示：

0 <= s.length <= 5 * 104
s 由英文字母、数字、符号和空格组成

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/longest-substring-without-repeating-characters
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution
{
public:
    /*
    int lengthOfLongestSubstring(string s) {
        int ans = 0;
        bool char_flag[256] = {false};
        int i = 0, j = 0;
        while(j < s.size()) {
            if (char_flag[s[j]]) {
                while(s[i] != s[j]) {
                    char_flag[s[i]] = false;
                    i++;
                }
                i++;
            }
            char_flag[s[j]] = true;
            ans = max(ans, j - i + 1);
            j++;
        }
        return ans;
    }
    */
    int lengthOfLongestSubstring(string s)
    {
        int ans = 0;
        int pos[256];
        memset(pos, 0xff, sizeof(int) * 256);
        int i = -1, j = 0;
        for (int j = 0; j < s.size(); j++)
        {
            i = max(pos[s[j]], i);
            pos[s[j]] = j;
            ans = max(ans, j - i);
        }
        return ans;
    }
};

void TestSolution()
{
    Solution s;
    cout << s.lengthOfLongestSubstring("bbbbb") << endl;
    cout << s.lengthOfLongestSubstring("") << endl;
    cout << s.lengthOfLongestSubstring("abcabcbb") << endl;
    cout << s.lengthOfLongestSubstring("pwwkew") << endl;
}