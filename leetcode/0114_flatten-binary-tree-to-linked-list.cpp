#include "common.h"

class Solution {
public:
    void flatten(TreeNode* root) {
        if (!root) {return;}
        flatten(root->left);
        flatten(root->right);
        if(root->left) {
            TreeNode* right_most = root->left;
            while(right_most->right) {
                right_most = right_most->right;
            }
            right_most->right = root->right;
            root->right = root->left;
            root->left = nullptr;
        }
    }
};