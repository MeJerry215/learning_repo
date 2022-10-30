#include "common.h"

class Solution
{
public:
    TreeNode *reverseOddLevels(TreeNode *root)
    {
        function<void(TreeNode*, TreeNode*, bool)> dfs = [&](TreeNode* lhs, TreeNode *rhs, bool is_reverse) -> void{
            if (!lhs) return;
            if (is_reverse) {
                swap(lhs->val, rhs->val);
            }
            dfs(lhs->left, rhs->right, !is_reverse);
            dfs(lhs->right, rhs->left, !is_reverse);
        };
        dfs(root->left, root->right, true);
        return root;
    }
};