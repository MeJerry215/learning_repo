#include "common.h"


/*
Given a binary string s and a positive integer n, return true if the binary representation of all the integers in the range [1, n] are substrings of s,
or false otherwise. A substring is a contiguous sequence of characters within a string.

Example 1:
Input: s = "0110", n = 3
Output: true
Example 2:
1
10
11

Input: s = "0110", n = 4
Output: false
1
10
11
100






Constraints:
1 <= s.length <= 1000
s[i] is either '0' or '1'.
1 <= n <= 109
*/


class Solution
{
public:
    bool queryString(string s, int n)
    {
        if (n > 1000) {
            return false;
        }

        for (int i = n; i > n / 2; --i) {
            string b = bitset<32>(i).to_string();
            b = b.substr(b.find_first_of('1'));

            if (s.find(b) == string::npos) {
                return false;
            }
        }

        return true;
    }
};


void TestSolution()
{
    Solution s;
    string str = "zhanglei";
    int n = 4;
    cout << s.queryString(str, n) << endl;
}