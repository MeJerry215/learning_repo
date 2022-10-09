#include "common.h"

class Solution
{
public:
    vector<vector<int>> largeGroupPositions_v1(string s)
    {
        int i = 1, repeat_cnt = 1;
        vector<vector<int>> res;
        while (i < s.size())
        {
            if (s[i] != s[i - 1]) {
                if (repeat_cnt >= 3) {
                    res.push_back({i - repeat_cnt, i - 1});
                }
                repeat_cnt = 1;
            } else {
                repeat_cnt++;
                if (i == s.size() - 1 && repeat_cnt >= 3) {
                    res.push_back({i - repeat_cnt + 1, i});
                }
            }
            i++;
        }
        return res;
    }


    vector<vector<int>> largeGroupPositions(string s) {
        vector<vector<int>> ret;
        int n = s.size();
        int num = 1;
        for (int i = 0; i < n; i++) {
            if (i == n - 1 || s[i] != s[i + 1]) {
                if (num >= 3) {
                    ret.push_back({i - num + 1, i});
                }
                num = 1;
            } else {
                num++;
            }
        }
        return ret;
    }

};

void TestSolution()
{
}