#include "common.h"

class Solution {
public:
    bool hasPathSum(TreeNode* root, int targetSum) {
        if (!root) {
            return false;
        }
        int res = targetSum - root->val;
        if (!root->left && !root->right && !res) {
            return true;
        }

        return hasPathSum(root->left, res) || hasPathSum(root->right, res);
    }
};