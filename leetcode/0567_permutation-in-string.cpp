#include "common.h"

/*
给你两个字符串 s1 和 s2 ，写一个函数来判断 s2 是否包含 s1 的排列。如果是，返回 true ；否则，返回 false 。
换句话说，s1 的排列之一是 s2 的 子串 。

示例 1：
输入：s1 = "ab" s2 = "eidbaooo"
输出：true
解释：s2 包含 s1 的排列之一 ("ba").
示例 2：
输入：s1= "ab" s2 = "eidboaoo"
输出：false
 

提示：

1 <= s1.length, s2.length <= 104
s1 和 s2 仅包含小写字母
通过次数230,194提交次数519,695

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/permutation-in-string
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution
{
public:
    bool checkInclusion(string s1, string s2)
    {
        constexpr int CHAR_NUM = 26;
        int char_cnts[CHAR_NUM] = {0};
        for (int i = 0; i < s1.size(); i++)
        {
            char_cnts[s1[i] - 'a']++;
        }
        int diff_cnt = 0;
        int k = s1.size();
        vector<int> res;
        for (int i = 0; i < s2.size(); i++)
        {
            if (i >= k)
            {
                char_cnts[s2[i - k] - 'a']++;
                if (char_cnts[s2[i - k] - 'a'] <= 0)
                    diff_cnt--;
            }
            char_cnts[s2[i] - 'a']--;
            if (char_cnts[s2[i] - 'a'] < 0)
                diff_cnt++;
            if (i >= k - 1 && diff_cnt == 0)
            {
                return true;
            }
        }
        return false;
    }
};