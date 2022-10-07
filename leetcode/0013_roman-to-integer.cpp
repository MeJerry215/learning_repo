#include "common.h"

class Solution {
public:
    int romanToInt_v1(string s) {
        unordered_map<char, int> char_maps = {
            {'I', 1}, {'V', 5}, {'X', 10}, {'L', 50}, {'C', 100}, {'D', 500}, {'M', 1000}
        };
        int res = 0;
        int tmp = 0;
        int last_base = char_maps[s[0]];
        for(int i = 0; i < s.size(); i++) {
            int num = char_maps[s[i]];
            if (num > last_base) {
                res -= tmp;
                tmp = num;
            } else if (num < last_base) {
                last_base = num;
                res += tmp;
                tmp = num;
            } else {
                tmp += num;
            }
            if (i == s.size() - 1) {
                res += tmp;
            }
        }
        return res;
    }

private:
    unordered_map<char, int> symbolValues = {
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'L', 50},
        {'C', 100},
        {'D', 500},
        {'M', 1000},
    };

public:
    int romanToInt(string s) {
        /* 读懂了题目, 大概才会变得下面这么简洁吧 */
        int ans = 0;
        int n = s.length();
        for (int i = 0; i < n; ++i) {
            int value = symbolValues[s[i]];
            if (i < n - 1 && value < symbolValues[s[i + 1]]) {
                ans -= value;
            } else {
                ans += value;
            }
        }
        return ans;
    }

};


void TestSolution() {
    Solution s;
    cout << s.romanToInt("III") << endl;
    cout << s.romanToInt("IV") << endl;
    cout << s.romanToInt("IX") << endl;
    cout << s.romanToInt("LVIII") << endl;
    cout << s.romanToInt("MCMXCIV") << endl;
}