#include "common.h"


class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        vector<int> ans;
        vector<vector<int>> res;
        function<void(int, int)> dfs = [&](int x, int remain) {
            if (remain == 0) {
                res.push_back(ans);
                return;
            }
            for(int i = x; i < candidates.size(); i++) {
                if (i > x && candidates[i] == candidates[i - 1]) {
                    continue;
                }
                if (remain - candidates[i] < 0) continue;
                ans.push_back(candidates[i]);
                dfs(i + 1, remain - candidates[i]);
                ans.pop_back();
            }
        };
        dfs(0, target);
        return res;
    }
};