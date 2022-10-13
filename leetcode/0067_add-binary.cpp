#include "common.h"

/*
给你两个二进制字符串 a 和 b ，以二进制字符串的形式返回它们的和。
示例 1：
输入:a = "11", b = "1"
输出："100"
示例 2：
输入：a = "1010", b = "1011"
输出："10101"

提示：
1 <= a.length, b.length <= 104
a 和 b 仅由字符 '0' 或 '1' 组成
字符串如果不是 "0" ，就不含前导零
*/

class Solution
{
public:
    string addBinary(string a, string b)
    {
        int carry = 0;
        /* make sure b string always longer than a string */
        if (a.size() > b.size()) {
            swap(a, b);
        }
        int i = a.size() - 1, j = b.size() - 1;
        while (i >= 0 || j >= 0) {
            int sum = 0;
            if (i >= 0 && j >= 0) {
                sum = a[i--] + b[j] - '0' * 2 + carry;
            } else {
                sum = b[j] - '0' + carry;
            }
            carry = sum / 2;
            sum = sum % 2;
            b[j--] = sum + '0';
        }
        if (carry) {
            return '1' + b;
        } else {
            return b;
        }
    }
};

void TestSolution()
{
    Solution s;
    string a = "1111111111";
    string b = "1";
    cout << s.addBinary(a, b) << endl;
}