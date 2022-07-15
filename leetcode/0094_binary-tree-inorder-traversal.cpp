#include "common.h"


class Solution {
public:
    void inorderTraversal_helper(TreeNode* root, vector<int>& ans) {
        if (root == nullptr) return;
        ans.push_back(root->val);
        inorderTraversal_helper(root->left, ans);
        inorderTraversal_helper(root->right, ans);
    }

    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> ans;
        inorderTraversal_helper(root, ans);
        return ans;
    }
};