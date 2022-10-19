#include "common.h"



/*
给你一个下标从 0 开始的字符串 word ，字符串只包含小写英文字母。你需要选择 一个 下标并 删除 下标处的字符，使得 word 中剩余每个字母出现 频率 相同。

如果删除一个字母后，word 中剩余所有字母的出现频率都相同，那么返回 true ，否则返回 false 。

注意：

字母 x 的 频率 是这个字母在字符串中出现的次数。
你 必须 恰好删除一个字母，不能一个字母都不删除。
 

示例 1：

输入：word = "abcc"
输出：true
解释：选择下标 3 并删除该字母，word 变成 "abc" 且每个字母出现频率都为 1 。
示例 2：

输入：word = "aazz"
输出：false
解释：我们必须删除一个字母，所以要么 "a" 的频率变为 1 且 "z" 的频率为 2 ，要么两个字母频率反过来。所以不可能让剩余所有字母出现频率相同。
 

提示：

2 <= word.length <= 100

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/remove-letter-to-equalize-frequency
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution {
public:
    bool equalFrequency(string word) {
        int cnt[26] = {0};
        for (char c : word) cnt[c - 'a']++;
        // 枚举字母 i 被删除
        for (int i = 0; i < 26; i++) if (cnt[i] > 0) {
            // 删除字母 i
            cnt[i]--;
            // 检查剩余字母的出现次数是否一致
            int t = 0;
            for (int j = 0; j < 26; j++) if (cnt[j] > 0) {
                if (t == 0) t = cnt[j];
                else if (t != cnt[j]) goto FAILED;
            }
            return true;
            // 恢复字母 i
            FAILED:
            cnt[i]++;
        }
        return false;
    }
};

