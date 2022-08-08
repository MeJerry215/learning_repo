#include "common.h"

class Solution {
public:
    bool isSymmetricHelper(TreeNode* left, TreeNode* right) {
        if ((!left && right) || (left && !right)) {
            return false;
        }
        if (!left && !right) {
            return true;
        }
        if (left->val != right->val) {
            return false;
        }

        return isSymmetricHelper(left->left, right->right) && isSymmetricHelper(left->right, right->left);
    }

    bool isSymmetric(TreeNode* root) {
        return isSymmetricHelper(root, root);
    }
};

