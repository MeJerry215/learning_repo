#include "common.h"

/*
给定两个字符串 s 和 p，找到 s 中所有 p 的 异位词 的子串，返回这些子串的起始索引。不考虑答案输出的顺序。
异位词 指由相同字母重排列形成的字符串（包括相同的字符串）。

示例 1:
输入: s = "cbaebabacd", p = "abc"
输出: [0,6]
解释:
起始索引等于 0 的子串是 "cba", 它是 "abc" 的异位词。
起始索引等于 6 的子串是 "bac", 它是 "abc" 的异位词。
 示例 2:
输入: s = "abab", p = "ab"
输出: [0,1,2]
解释:
起始索引等于 0 的子串是 "ab", 它是 "ab" 的异位词。
起始索引等于 1 的子串是 "ba", 它是 "ab" 的异位词。
起始索引等于 2 的子串是 "ab", 它是 "ab" 的异位词。

提示:
1 <= s.length, p.length <= 3 * 104
s 和 p 仅包含小写字母

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/find-all-anagrams-in-a-string
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution
{
public:
    /*
    使用固定大小的的滑动窗口，统计有多少元素的差异。
    a b c a a a b     expect "aba"
    a b c             a b的个数都不大于 计数，  c的计数大于了计数 "aba"中c的个数, 产生了diff  diff = 1
      b c a           a 被踢出去了, 所以a的计数减少了，什么情况下需要统a产生的diff，就是之前的a的计数大于预期需要的a的个数产生了diff, 也就是通过 上一步右滑添加进来的, 所以此次无diff新增
        c a a         a 的计数 2, 没有超过，所以没有diff新增, diff = 1, b被踢出去，但是b没有贡献过diff，所以不关注
          a a a       a 的计数 3, 超过, 产生了1的diff。 同时踢出去了c 导致 diff减小， diff = 1 + 1 - 1
            a a b     c 被踢出去了，减去之前贡献的diff, 新增的a没有产生diff， 所以diff = 1 - 1 = 0
    */

    vector<int> findAnagrams_v1(string s, string p)
    {
        unordered_map<char, int> char_cnts;
        for (char c : p) {
            char_cnts[c]++;
        }
        int k = p.size();
        unordered_map<int, int> s_cnt;
        vector<int> res;
        int diff_cnt = 0;
        for (int i = 0; i < s.size(); i++) {
            if (i >= k) {
                s_cnt[s[i - k]]--;
                if (s_cnt[s[i - k]] >= char_cnts[s[i - k]])
                    diff_cnt--;
            }
            s_cnt[s[i]]++;
            if (s_cnt[s[i]] > char_cnts[s[i]])
                diff_cnt++;
            if (i >= k - 1 && diff_cnt == 0) {
                res.push_back(i - k + 1);
            }
        }
        return res;
    }

    /*
    在上面的场景，看上去性能并不是很好，只有40%, 所以思考如何优化掉 hash_map的开销，直接使用数组来统计。
    上面的结果将hash 数组的比较，直接看做是 0 > char_cnts[s[i]] - s_cnt[s[i]], p ++ 计数，s -- 计数， 所以合并为char_cnts[s[i] - 'a'] < 0
    */

    vector<int> findAnagrams(string s, string p)
    {
        constexpr int CHAR_NUM = 26;
        int char_cnts[CHAR_NUM] = {0};
        for(int i = 0; i < p.size(); i++) {
            char_cnts[p[i] - 'a']++;
        }
        int diff_cnt = 0;
        int k = p.size();
        vector<int> res;
        for(int i = 0; i < s.size(); i++) {
            if (i >= k) {
                char_cnts[s[i - k] - 'a'] ++;
                if (char_cnts[s[i - k] - 'a'] <= 0) diff_cnt--;
            }
            char_cnts[s[i] - 'a'] --;
            if (char_cnts[s[i] - 'a'] < 0) diff_cnt ++;
            if (i >= k - 1 && diff_cnt == 0)
            {
                res.push_back(i - k + 1);
            }
        }
        return res;
    };
};