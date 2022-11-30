#include "common.h"

/*
给你一个字符串 s 和一个 长度相同 的整数数组 indices 。
请你重新排列字符串 s ，其中第 i 个字符需要移动到 indices[i] 指示的位置。

返回重新排列后的字符串。

 

示例 1：



输入：s = "codeleet", indices = [4,5,6,7,0,2,1,3]
输出："leetcode"
解释：如图所示，"codeleet" 重新排列后变为 "leetcode" 。
示例 2：

输入：s = "abc", indices = [0,1,2]
输出："abc"
解释：重新排列后，每个字符都还留在原来的位置上。

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/shuffle-string
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    string restoreString(string s, vector<int>& indices) {
        for(int i = 0; i < s.size(); i++) {
            if (indices[i] == INT_MAX) continue;
            char c1 = s[i], c2;
            int t = i, q = indices[i];
            while(q != INT_MAX) {
                c2 = s[q];
                s[q] = c1;
                indices[t] = INT_MAX;
                t = q;
                q = indices[t];
                c1 = c2;
            }
        }
        return s;
    }

    string restoreString(string s, vector<int>& indices) {
        int length = s.length();
        for (int i = 0; i < length; i++) {
            if (indices[i] != i) {
                char ch = s[i]; // 当前需要被移动的字符
                int idx = indices[i]; // 该字符需要被移动的目标位置
                while (idx != i) {
                    swap(s[idx], ch); // 使用 swap 函数，在覆写 s[idx] 之前，先将其原始值赋给变量 ch
                    swap(indices[idx], idx); // 将封闭路径中的 indices 数组的值设置成下标自身
                }
                // 退出循环后，还要再覆写起点处的字符

                s[i] = ch;
                indices[i] = i;
            }
        }
        return s;
    }
    
};