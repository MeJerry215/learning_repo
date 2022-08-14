#include "common.h"

/*
给定一个不含重复数字的数组 nums ，返回其 所有可能的全排列 。你可以 按任意顺序 返回答案。

示例 1：
输入：nums = [1,2,3]
输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
示例 2：
输入：nums = [0,1]
输出：[[0,1],[1,0]]
示例 3：
输入：nums = [1]
输出：[[1]]
提示：
1 <= nums.length <= 6
-10 <= nums[i] <= 10
nums 中的所有整数 互不相同

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/permutations
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    void dfs_permute(vector<int>& nums, vector<int>& used, vector<vector<int>>& ans) {
        if (used.size() == nums.size()) {
            ans.push_back(vector<int>(used.begin(), used.end()));
            return;
        }
        for(int i = 0; i < nums.size(); i++) {
            if(find(used.begin(), used.end(), nums[i]) != used.end()) {
                continue;
            }
            used.push_back(nums[i]);
            dfs_permute(nums, used, ans);
            used.pop_back();
        }
    }

    vector<vector<int>> permute(vector<int>& nums) {
        vector<int> used;
        vector<vector<int>> ans;
        dfs_permute(nums, used, ans);
        return ans;
    }
};