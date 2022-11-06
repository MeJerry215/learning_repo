#include "common.h"

class Solution {
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> res;
        vector<int> ans(k, 0);
        function<void(int, int)> dfs = [&](int x, int s) {
            if (s == k) {
                res.push_back(ans);
                return;
            }
            for(int i = x; i <= n; i++) {
                ans[s] = i;
                dfs(i + 1, s + 1);
            }
        };
        dfs(1, 0);
        return res;
    }
};