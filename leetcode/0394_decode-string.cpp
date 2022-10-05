#include "common.h"

/*
给定一个经过编码的字符串，返回它解码后的字符串。
编码规则为: k[encoded_string]，表示其中方括号内部的 encoded_string 正好重复 k 次。注意 k 保证为正整数。
你可以认为输入字符串总是有效的；输入字符串中没有额外的空格，且输入的方括号总是符合格式要求的。
此外，你可以认为原始数据不包含数字，所有的数字只表示重复的次数 k ，例如不会出现像 3a 或 2[4] 的输入。

示例 1：
输入：s = "3[a]2[bc]"
输出："aaabcbc"
示例 2：
输入：s = "3[a2[c]]"
输出："accaccacc"
示例 3：
输入：s = "2[abc]3[cd]ef"
输出："abcabccdcdcdef"
示例 4：
输入：s = "abc3[cd]xyz"
输出："abccdcdcdxyz"

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/decode-string
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    /* 这道题考察毡的用法
    以  3[a2[c]] 为例，分为数字栈和符号栈，符号栈主要保存的是字符和 []
    入3 数字
    3   
    入符号[
    3  [
    入a
    3  [ a
    入2
    3 2   [ a
    入[
    3 2   [ a [
    入c
    3 2   [ a [ c
    遇到 ]
    出字符栈到[ 同时出栈字符相加, 最后出个数字
    3     [ a              cc
    遇到 ]
    accaccacc
    */


    string decodeString(string s)
    {
        int n = s.size();
        vector<string> strs;
        vector<int> nums;
        for (int i = 0; i < n; i++)
        {
            if (isdigit(s[i]))
            {
                int j = i + 1;
                while (j < n && isdigit(s[j]))
                {
                    j++;
                }
                nums.push_back(atoi(s.substr(i, j - i).c_str()));
                i = j - 1;
            }
            else if (s[i] == '[')
            {
                strs.push_back("");
            }
            else if (s[i] == ']')
            {
                // take one num
                int repeat = nums.back();
                nums.pop_back();
                string tmp = "";
                while (true)
                {
                    string top_str = strs.back();
                    if (top_str.size() == 0)
                    {
                        strs.pop_back();
                        break;
                    }
                    tmp = top_str + tmp;
                    strs.pop_back();
                }
                string repeat_tmp = "";
                for (int j = 0; j < repeat; j++)
                {
                    repeat_tmp += tmp;
                }
                strs.push_back(repeat_tmp);
            }
            else
            {
                int j = i + 1;
                while (j < n && isalpha(s[j]))
                {
                    j++;
                }
                strs.push_back(s.substr(i, j - i));
                i = j - 1;
            }
        }
        return accumulate(strs.begin(), strs.end(), string());
    }
};