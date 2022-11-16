#include "common.h"

/*
给定两个字符串 order 和 s 。order 的所有字母都是 唯一 的，并且以前按照一些自定义的顺序排序。
对 s 的字符进行置换，使其与排序的 order 相匹配。更具体地说，如果在 order 中的字符 x 出现字符 y 之前，那么在排列后的字符串中， x 也应该出现在 y 之前。
返回 满足这个性质的 s 的任意一种排列 。

示例 1:
输入: order = "cba", s = "abcd"
输出: "cbad"
解释: 
“a”、“b”、“c”是按顺序出现的，所以“a”、“b”、“c”的顺序应该是“c”、“b”、“a”。
因为“d”不是按顺序出现的，所以它可以在返回的字符串中的任何位置。“dcba”、“cdba”、“cbda”也是有效的输出。
示例 2:
输入: order = "cbafg", s = "abcd"
输出: "cbad"

提示:
1 <= order.length <= 26
1 <= s.length <= 200
order 和 s 由小写英文字母组成
order 中的所有字符都 不同

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/custom-sort-string
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    string customSortString(string order, string s) {
        unordered_map<char, int> order_dict;
        for(int i = 1; i <= order.size(); i++) {
            order_dict[order[i - 1]] = i;
        }
        sort(s.begin(), s.end(), [&](char l, char r) {
            return order_dict[l] < order_dict[r];
        });
        return s;
    }
};