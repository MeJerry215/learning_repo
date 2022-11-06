#include "common.h"


class Solution {
public:

    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> ans;
        sort(nums.begin(), nums.end());
        function<void()> dfs = [&]() {
            if (ans.size() == nums.size()) {
                res.push_back(ans);
                return;
            }
            for(int i = 0; i < nums.size(); i++) {
                if (nums[i] == INT_MAX) continue;;
                if (i > 0 && nums[i] == nums[i - 1]) continue;
                int val = nums[i];
                ans.push_back(val);
                nums[i] = INT_MAX;
                dfs();
                nums[i] = val;
                ans.pop_back();
            }
        };
        dfs();
        return res;
    }
};