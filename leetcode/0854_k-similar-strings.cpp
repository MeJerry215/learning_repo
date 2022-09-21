#include "common.h"

class Solution
{
public:
    int kSimilarity(string s1, string s2) {
        int res = s1.size() - 1;
        function<void(int, int)> dfs = [&](int cur, int ans) {
            if  (ans >= res) {
                return;
            }
            if (cur == s1.size()) {
                res = min(ans, res);
            }
            if (s1[cur] == s2[cur]) {
                dfs(cur + 1, ans);
            } else {
                for(int i = cur + 1; i < s1.size(); i ++) {
                    if (s2[i] != s1[i] && s1[cur] == s2[i]) {
                        swap(s2[i], s2[cur]);
                        dfs(cur + 1, ans + 1);
                        swap(s2[i], s2[cur]);
                    }
                }
            }

        };
        dfs(0, 0);
        return res;
    }
};


void TestSolution() {
    Solution s;
    string s1 = "abcdef";
    string s2 = "efabcd";
    cout << s.kSimilarity(s1, s2) << endl;
}