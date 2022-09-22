#include "common.h"

class Solution {
public:

    bool isSymmetricHelper(TreeNode* left, TreeNode* right) {
        if (!left && !right) {
            return true;
        }
        if (left && right) {
            if (left->val != right->val) return false;
            return isSymmetricHelper(left->left, right->right) && isSymmetricHelper(left->right, right->left);
        }
        return false;
    }

    bool isSymmetric(TreeNode* root) {
        if (!root) return true;
        return isSymmetricHelper(root->left, root->right);
    }
};