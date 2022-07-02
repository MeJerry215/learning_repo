#include "common.h"

class Solution
{
public:
    void find_palindrome(const string &s, int l, int r, int &la, int &ans_size)
    {
        while (l >= 0 && r < s.size())
        {
            if (s[l] == s[r])
            {
                if ((r - l + 1) > ans_size)
                {
                    la = l, ans_size = (r - l + 1);
                }
                l--, r++;
                continue;
            }
            break;
        }
    }

    string longestPalindrome(string s)
    {
        string ans = "";
        int l, r;
        int la;
        int ans_size = 0;
        if (s.size() < 2)
            return s;
        for (int i = 0; i < s.size() - 1; i++)
        {
            // case aba
            l = r = i;
            find_palindrome(s, l, r, la, ans_size);
            // case abba
            l = i, r = i + 1;
            find_palindrome(s, l, r, la, ans_size);
        }
        return s.substr(la, ans_size);
    }
};

void TestSolution()
{
    Solution s;
    string str = "aba";
    cout << s.longestPalindrome(str) << endl;
    str = "babad";
    cout << s.longestPalindrome(str) << endl;
    str = "";
    cout << s.longestPalindrome(str) << endl;
    str = "aacabdkacaa";
    cout << s.longestPalindrome(str) << endl;
}