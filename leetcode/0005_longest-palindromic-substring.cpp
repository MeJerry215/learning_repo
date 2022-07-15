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

    string longestPalindrome_dp(string s) {
        int size = s.size();
        if (size < 2) {
            return s;
        }
        bool dp[size][size];
        for (int i = 0; i < size; i++) {
            dp[i][i] = true;
        }
        int start = 0, len = 1;
        for (int j = 1; j < size; j++) {
            for(int i = 0; i < j; i++) {
                if (s[i] != s[j]) {
                    dp[i][j] = false;
                } else {
                    if (j - i < 3 || dp[i+1][j - 1]) {
                        dp[i][j] = true;
                    } else {
                        dp[i][j] = false;
                    }
                }
                if (dp[i][j] && (j - i + 1) > len) {
                    len = j - i + 1;
                    start = i;
                }
            }
        }
        return s.substr(start, len);
    }
};


void TestSolution() {
    Solution s;
    cout << s.longestPalindrome_dp("aba") << endl;
    cout << s.longestPalindrome_dp("cbbd") << endl;
}