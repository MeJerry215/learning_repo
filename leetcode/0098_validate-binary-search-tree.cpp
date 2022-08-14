#include "common.h"

class Solution {
public:
    bool isValidBSTHelper(TreeNode* root, int64_t up_bound, int64_t low_bound) {
        if (!root) {
            return true;
        }
        if (root->val >= up_bound || root->val <= low_bound) {
            return false;
        }
        bool valid = isValidBSTHelper(root->left, root->val, low_bound) && isValidBSTHelper(root->right, up_bound, root->val);
        if (root->left && root->val <= root->left->val) {
            return false;
        }
        if (root->right && root->val >= root->right->val) {
            return false;
        }

        return valid;
    }


    bool isValidBST(TreeNode* root) {
        return isValidBSTHelper(root, INT64_MAX, INT64_MIN);
    }
};


