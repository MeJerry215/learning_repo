#include "common.h"

/*
给你一个 无重复元素 的整数数组 candidates 和一个目标整数 target ，找出 candidates 中可以使数字和为目标数 target 的 所有 不同组合 ，并以列表形式返回。你可以按 任意顺序 返回这些组合。
candidates 中的 同一个 数字可以 无限制重复被选取 。如果至少一个数字的被选数量不同，则两种组合是不同的。 
对于给定的输入，保证和为 target 的不同组合数少于 150 个。

示例 1：
输入：candidates = [2,3,6,7], target = 7
输出：[[2,2,3],[7]]
解释：
2 和 3 可以形成一组候选，2 + 2 + 3 = 7 。注意 2 可以使用多次。
7 也是一个候选， 7 = 7 。
仅有这两种组合。
示例 2：
输入: candidates = [2,3,5], target = 8
输出: [[2,2,2,2],[2,3,3],[3,5]]
示例 3：
输入: candidates = [2], target = 1
输出: []
提示：
1 <= candidates.length <= 30
1 <= candidates[i] <= 200
candidate 中的每个元素都 互不相同
1 <= target <= 500

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/combination-sum
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/


class Solution {
public:
    void dfs_combination_sum(vector<int>& cadidates, int target, int cur, int idx, unordered_set<string>& contains, vector<vector<int>> &ans, vector<int>& nums) {
        if (cur == target) {
            string key(nums.begin(), nums.end());
            if (contains.find(key) == contains.end()) {
                ans.push_back(nums);
                contains.insert(key);
                return;
            }
        }
        for(int i = idx; i < cadidates.size(); i++) {
            cur += cadidates[i];
            if (cur > target) {break;}
            nums.push_back(cadidates[i]);
            dfs_combination_sum(cadidates, target, cur, i, contains, ans, nums);
            cur -= cadidates[i];
            nums.pop_back();
        }
    }

    int k = 0;
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        vector<vector<int>> ans;
        unordered_set<string> contains;
        vector<int> nums;
        dfs_combination_sum(candidates, target, 0, 0, contains, ans, nums);
        return ans;
    }
};