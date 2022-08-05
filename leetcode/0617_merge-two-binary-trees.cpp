#include "common.h"

class Solution {
public:
    TreeNode* mergeTreesHelper(TreeNode* root1, TreeNode* root2) {
        if (!root1 && !root2) {
            return nullptr;
        }
        if (root1 && root2) {
            root1->val += root2->val;
            root1->left = mergeTreesHelper(root1->left, root2->left);
            root1->right = mergeTreesHelper(root1->right, root2->right);
            return root1;
        } else if(root1) {
            return root1;
        } else {
            return root2;
        }
    }

    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        return mergeTreesHelper(root1, root2);
    }
};