#include "common.h"


/*
你会得到一个字符串 s (索引从 0 开始)，你必须对它执行 k 个替换操作。替换操作以三个长度均为 k 的并行数组给出：indices, sources,  targets。

要完成第 i 个替换操作:

检查 子字符串  sources[i] 是否出现在 原字符串 s 的索引 indices[i] 处。
如果没有出现， 什么也不做 。
如果出现，则用 targets[i] 替换 该子字符串。
例如，如果 s = "abcd" ， indices[i] = 0 , sources[i] = "ab"， targets[i] = "eee" ，那么替换的结果将是 "eeecd" 。

所有替换操作必须 同时 发生，这意味着替换操作不应该影响彼此的索引。测试用例保证元素间不会重叠 。

例如，一个 s = "abc" ，  indices = [0,1] ， sources = ["ab"，"bc"] 的测试用例将不会生成，因为 "ab" 和 "bc" 替换重叠。
在对 s 执行所有替换操作后返回 结果字符串 。

子字符串 是字符串中连续的字符序列。

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/find-and-replace-in-string
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/


class Solution {
public:
    string findReplaceString(string s, vector<int>& indices, vector<string>& sources, vector<string>& targets) {
        vector<int> indexes(indices.size());
        iota(indexes.begin(), indexes.end(), 0);
        vector<bool> found(indices.size(), false);
        sort(indexes.begin(), indexes.end(), [&](const int i, const int j) {
            return indices[i] <indices[j];
        });
        string res = "";
        for (int index: indexes) {
            found[index] = s.find(sources[index], indices[index]) == indices[index];
        }
        int cur = 0, start = 0;
        while(start < s.size()) {
            if (cur < indexes.size() && start == indices[indexes[cur]]) {
                if (found[indexes[cur]]) {
                    res += targets[indexes[cur]];
                    start += sources[indexes[cur]].size();
                }
                cur ++;
            } else {
                res += s[start++];
            }
        }
        return res;
    }
};


// "mhnbzxkwzxtaanmhtoirxheyanoplbvjrovzudznmetkkxrdmr"
// [46,29,2,44,31,26,42,9,38,23,36,12,16,7,33,18]
// ["rym","kv","nbzxu","vx","js","tp","tc","jta","zqm","ya","uz","avm","tz","wn","yv","ird"]
// ["gfhc","uq","dntkw","wql","s","dmp","jqi","fp","hs","aqz","ix","jag","n","l","y","zww"]
// "mhnbzxkwzxtaanmhtoirxheyanoplbvjrovzudznmetkkxrdmr"
// "mhnbzxkwzxtaanmhtoirxhenoplbvjrovzudznmetkkxrdmr"


// void Tes