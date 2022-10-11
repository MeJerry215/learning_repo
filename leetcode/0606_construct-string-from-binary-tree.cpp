#include "common.h"

class Solution {
public:
    string tree2str(TreeNode* root) {
    if (!root) return "";
    auto lhs = tree2str(root->left);
    auto rhs = tree2str(root->right);
    if (root->right) {
      return to_string(root->val) + "(" + lhs + ")(" + rhs + ")";
    }
    if (root->left) {
      return to_string(root->val) + "(" + lhs + ")";
    }
    return to_string(root->val);
  }
};