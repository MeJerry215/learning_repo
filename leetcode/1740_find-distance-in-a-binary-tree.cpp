#include "common.h"

class Solution {
public:
    int findDistance(TreeNode* root, int p, int q) {
        int res = 0;
        function<int(TreeNode*)> dfs = [&](TreeNode* node) -> int {
            if (!node) return 0;
            int l = dfs(node->left);
            int r = dfs(node->right);
            if (node->val != p && node->val != q) {
                if (l != 0 && r != 0) {
                    res = l + r;
                } else if (l != 0 ||  r != 0) return max(l, r) + 1;
            } else {
                res = max(l, r);
                return 1;
            }
            return 0;
        };
        dfs(root);
        return res;
    }
};