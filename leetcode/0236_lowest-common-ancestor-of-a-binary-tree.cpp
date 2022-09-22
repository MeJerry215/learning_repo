#include "common.h"

class Solution {
public:
    TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {
        TreeNode *ans = nullptr;
        function<TreeNode *(TreeNode *)> dfs = [&](TreeNode *node) -> TreeNode * {
            if (!node || ans) {
                return nullptr;
            }
            TreeNode *l = dfs(node->left);
            TreeNode *r = dfs(node->right);
            bool selfEqual = node->val == p->val || node->val == q->val;
            if (l && r || (selfEqual && (l || r))) {
                ans = node;
                return ans;
            }
            if (selfEqual || (l || r)) return node;
            return nullptr;
        };
        dfs(root);
        return ans;
    }
};

// [3,5,1,6,2,0,8,null,null,7,4]
// 5
// 4


                        3
                5               1
            6       2       0       8
                  7   4