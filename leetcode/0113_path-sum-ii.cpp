#include "common.h"

class Solution {
public:
    vector<vector<int>> ans;
    void pathSumHelper(TreeNode* root, int res, vector<int>& nums) {
        if (!root) {return;}
        res -= root->val;
        nums.push_back(root->val);
        if (!res && !root->left && !root->right) {
            ans.push_back(nums);
            nums.pop_back();
            return;
        }
        pathSumHelper(root->left, res, nums);
        pathSumHelper(root->right, res, nums);
        nums.pop_back();
    }

    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        vector<int> cached;
        pathSumHelper(root, targetSum, cached);
        return ans;
    }
};