#include "common.h"

class Solution {
public:
    TreeNode* trimBSHelper(TreeNode* root, int low, int high)  {
        if (!root) {
            return nullptr;
        }
        if (root->val >= low && root->val <= high) {
            root->left = trimBSHelper(root->left, low, high);
            root->right = trimBSHelper(root->right, low, high);
            return root;
        } else {
            if (root->val < low) {
                return trimBSHelper(root->right, low, high);
            } else {
                return trimBSHelper(root->left, low, high);
            }
        }
    }

    TreeNode* trimBST(TreeNode* root, int low, int high) {
        return trimBSHelper(root, low, high);
    }
};