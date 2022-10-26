#include "common.h"

/*
给你一个二进制字符串 s 。如果字符串中由 1 组成的 最长 连续子字符串 严格长于 由 0 组成的 最长 连续子字符串，返回 true ；否则，返回 false 。
例如，s = "110100010" 中，由 1 组成的最长连续子字符串的长度是 2 ，由 0 组成的最长连续子字符串的长度是 3 。
注意，如果字符串中不存在 0 ，此时认为由 0 组成的最长连续子字符串的长度是 0 。字符串中不存在 1 的情况也适用此规则。
示例 1：

输入：s = "1101"
输出：true
解释：
由 1 组成的最长连续子字符串的长度是 2："1101"
由 0 组成的最长连续子字符串的长度是 1："1101"
由 1 组成的子字符串更长，故返回 true 。
示例 2：
输入：s = "111000"
输出：false
解释：
由 1 组成的最长连续子字符串的长度是 3："111000"
由 0 组成的最长连续子字符串的长度是 3："111000"
由 1 组成的子字符串不比由 0 组成的子字符串长，故返回 false 。
示例 3：
输入：s = "110100010"
输出：false
解释：
由 1 组成的最长连续子字符串的长度是 2："110100010"
由 0 组成的最长连续子字符串的长度是 3："110100010"
由 1 组成的子字符串不比由 0 组成的子字符串长，故返回 false 。

提示：

1 <= s.length <= 100
s[i] 不是 '0' 就是 '1'

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/longer-contiguous-segments-of-ones-than-zeros
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    bool checkZeroOnes(string s) {
        int zero_max = 0;
        int one_max = 0;
        int zero_cnt = 0;
        int one_cnt = 0;
        for(int i = 0; i < s.size(); i++) {
            if (s[i] == '1') {
                zero_cnt = 0;
                one_cnt ++;
                one_max = max(one_max, one_cnt);
            } else {
                one_cnt = 0;
                zero_cnt ++;
                zero_max = max(zero_max, zero_cnt);
            }
        }
        return zero_max < one_max;
    }
};