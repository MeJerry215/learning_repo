#include "common.h"

/*
将一个给定字符串 s 根据给定的行数 numRows ，以从上往下、从左到右进行 Z 字形排列。
比如输入字符串为 "PAYPALISHIRING" 行数为 3 时，排列如下：

P   A   H   N
A P L S I I G
Y   I   R
之后，你的输出需要从左往右逐行读取，产生出一个新的字符串，比如："PAHNAPLSIIGYIR"。
请你实现这个将字符串进行指定行数变换的函数：
string convert(string s, int numRows);
 

示例 1：
输入：s = "PAYPALISHIRING", numRows = 3
输出："PAHNAPLSIIGYIR"
示例 2：
输入：s = "PAYPALISHIRING", numRows = 4
输出："PINALSIGYAHRPI"
解释：
P     I    N
A   L S  I G
Y A   H R
P     I
示例 3：
输入：s = "A", numRows = 1
输出："A"
 

提示：
1 <= s.length <= 1000
s 由英文字母（小写和大写）、',' 和 '.' 组成
1 <= numRows <= 1000
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/zigzag-conversion
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    string convert2(string s, int numRows) {
        vector<vector<int>> num_buffer(numRows, vector<int>());
        int j = 0;
        if (numRows == 1) {
            return s;
        }
        bool inverse_flag = true;
        for (int i = 0; i < s.size(); i++) {
            if (j == numRows - 1) {
                inverse_flag = true;
            } else if (j == 0) {
                inverse_flag = false;
            }
            num_buffer[j].push_back(s[i]);
            inverse_flag ? j-- : j++;
        }
        string ans = "";
        for (int i = 0; i < numRows; i++) {
            ans += string(num_buffer[i].begin(), num_buffer[i].end());
        }
        return ans;
    }
/*
0             0+t                    0+2t                     0+3t
1      t-1    1+t            0+2t-1  1+2t            0+3t-1   1+3t
2  t-2        2+t  0+2t-2            2+2t  0+3t-2             2+3t  
3             3+t                    3+2t
*/
    string convert(string s, int numRows) {
        int t = 2 * numRows - 2;
        string ans = "";
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j + i < s.size(); j += t) {
                ans += s[j + i];
                if (0 < i && i < numRows - 1 && j + t - i < s.size()) {
                    ans += s[j + t - i];
                }
            }
        }
        return ans;
    }
};


void TestSolution() {
    Solution s;
    cout << s.convert("PAYPALISHIRING", 3) << endl;
    cout << "PAHNAPLSIIGYIR" << endl;
    cout << s.convert("PAYPALISHIRING", 4) << endl;
    cout << "PINALSIGYAHRPI" << endl;
    // static_assert(s.con)
}