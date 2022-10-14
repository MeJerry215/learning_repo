#include "common.h"


/*
给定一个字符串 s，计算 s 的 不同非空子序列 的个数。因为结果可能很大，所以返回答案需要对 10^9 + 7 取余 。
字符串的 子序列 是经由原字符串删除一些（也可能不删除）字符但不改变剩余字符相对位置的一个新字符串。
例如，"ace" 是 "abcde" 的一个子序列，但 "aec" 不是。

示例 1：
输入：s = "abc"
输出：7
解释：7 个不同的子序列分别是 "a", "b", "c", "ab", "ac", "bc", 以及 "abc"。
示例 2：
输入：s = "aba"
输出：6
解释：6 个不同的子序列分别是 "a", "b", "ab", "ba", "aa" 以及 "aba"。
示例 3：
输入：s = "aaa"
输出：3
解释：3 个不同的子序列分别是 "a", "aa" 以及 "aaa"。

提示：
1 <= s.length <= 2000
s 仅由小写英文字母组成

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/distinct-subsequences-ii
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

/*
分析：问题是求解字符串的字序列个数，所以如果考虑通常的dfs求解，则每一位存在两种选择，选择和不选择，根据题目意思s.length 可以到2000, 所以O(2^n)并不是一个小的数量级，而同时需要考虑去重，则时间复杂度和空间复杂度都不可取。

所以根据问题来看这道题，应该可能存在一个递推关系，来计算得到下一个序列的

需要分解问题来看，假设字符串
    abcacc

    a       ->  "" |
                "a"     1 + 1 - 0 = 2
    ab      ->  ""  "a" |
                "b" "ab"     2 + 2 - 0 = 4
    abc     ->  ""  "a"   "b"  "ab" |
                "c" "ac"  "bc" "abc"    4 + 4 - 0 = 8
    abca    ->  ""  "a"   "b"  "ab" "c"  "ac"  "bc"  "abc" |
               ("a")"aa" "ba" "aba" "ca" "aca" "bca" "abca"   = 8 + 8 - 1 = 15
    abcac   ->  ""  "a"   "b"  "ab"  "c"  "ac"  "bc"  "abc"  ("a")  "aa"  "ba"  "aba"  "ca"  "aca"  "bca"  "abca" |
               ("c" "ac" "bc" "acb") "cc" "acc" "bcc" "abcc" ("ac") "aac" "bac" "abac" "cac" "acac" "bcac" "abcac" = 15 + 15 - 4 = 26
    abcacc  ->  ""  "a"   "b"  "ab"   "c"  "ac"  "bc"  "abc"  ("a") "aa"  "ba"  "aba"  "ca"  "aca"  "bca"  "abca"  ("c" "ac"   "bc"  "acb")  "cc"  "acc"  "bcc"  "abcc"  ("ac")  "aac"  "bac"  "abac"  "cac"  "acac"  "bcac" "abcac" |
               ("c" "ac" "bc" "abc"  "cc" "acc" "bcc" "abcc" ("ac") "aac" "bac" "abac" "cac" "acac" "bcac" "abcac" ("cc" "acc" "bcc" "acbc")) "ccc" "accc" "bccc" "abccc" ("acc") "aacc" "bacc" "abacc" "cacc" "acacc" "bcac" "abcacc" 26 + 26 - 15 = 37
    abcacc  = 37 + 37 - 26 = 48

*/

class Solution {
    static constexpr int mod = 1000000007;
public:
    int distinctSubseqII(string s) {
        vector<long> increase(26, 0);
        int total = 1;
        for(int i = 0; i < s.size(); i++) {
            int prev = increase[s[i] - 'a'];
            increase[s[i] - 'a'] = total;
            total = ((total * 2 - prev) % mod + mod) % mod;
        }
        return total - 1;
    }
};