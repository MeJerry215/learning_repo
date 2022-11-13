#include "common.h"

/*
给你一个字符串 message 和一个正整数 limit 。
你需要根据 limit 将 message 分割 成一个或多个 部分 。每个部分的结尾都是 "<a/b>" ，其中 "b" 用分割出来的总数 替换， "a" 用当前部分所在的编号 替换 ，编号从 1 到 b 依次编号。除此以外，除了最后一部分长度 小于等于 limit 以外，其他每一部分（包括结尾部分）的长度都应该 等于 limit 。
你需要确保分割后的结果数组，删掉每部分的结尾并 按顺序 连起来后，能够得到 message 。同时，结果数组越短越好。
请你返回 message  分割后得到的结果数组。如果无法按要求分割 message ，返回一个空数组。

示例 1：
输入：message = "this is really a very awesome message", limit = 9
输出：["thi<1/14>","s i<2/14>","s r<3/14>","eal<4/14>","ly <5/14>","a v<6/14>","ery<7/14>"," aw<8/14>","eso<9/14>","me<10/14>"," m<11/14>","es<12/14>","sa<13/14>","ge<14/14>"]
解释：
前面 9 个部分分别从 message 中得到 3 个字符。
接下来的 5 个部分分别从 message 中得到 2 个字符。
这个例子中，包含最后一个部分在内，每个部分的长度都为 9 。
可以证明没有办法分割成少于 14 个部分。
示例 2：

输入：message = "short message", limit = 15
输出：["short mess<1/2>","age<2/2>"]
解释：
在给定限制下，字符串可以分成两个部分：
- 第一个部分包含 10 个字符，长度为 15 。
- 第二个部分包含 3 个字符，长度为 8 。
 

提示：

1 <= message.length <= 104
message 只包含小写英文字母和 ' ' 。
1 <= limit <= 104

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/split-message-based-on-limit
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    vector<string> splitMessage(string message, int limit) {
        int n = message.size();
        int num_len[n + 1];

        for(int i = 1, cap = 0, tail_len = 0; i <= message.size(); i++) {
            if (i < 10) {
                tail_len = 5;
            } else if (i < 100) {
                if (i == 10) cap -= 9;
                tail_len = 7;
            } else if (i < 1000) {
                if (i == 100) cap -= 99;
                tail_len = 9;
            } else {
                if (i == 1000) cap -= 999;
                tail_len = 11;
            }
            if (tail_len > limit) return {};
            cap += limit - tail_len;
            if (cap < n) continue;
            vector<string> res;
            int pos = 0;
            function<string(int, int, bool)> make_str = [&](int start, int part, bool last_part) {
                string tail = "<" + to_string(part) + "/" + to_string(i) + ">";
                pos += (limit - tail.size());
                return last_part ? message.substr(start) + tail : message.substr(start, limit - tail.size()) + tail;
            };
            for(int j = 1; j <= i; j++) {
                res.push_back(make_str(pos, j, j == i));
            }
            return res;
        }
        return {};
    }
};


void TestSolution() {
    Solution s;
    string message = "p";
    auto res = s.splitMessage(message, 7);
    print_vec(res);
}