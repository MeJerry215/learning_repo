#include "common.h"

class Solution {
public:
    /*
    如果左右子树返回都是0, 那么不递树的高度。
    */
    int findDistance(TreeNode* root, int p, int q) {
        int res = 0;
        function<int(TreeNode*)> dfs = [&](TreeNode* node) -> int {
            if (!node) return 0;
            int l = dfs(node->left);
            int r = dfs(node->right);
            // 当前节点不是要找的节点
            if (node->val != p && node->val != q) {
                // 左右子树都有高度，直接就是答案
                if (l != 0 && r != 0) {
                    res = l + r;
                } else if (l != 0 ||  r != 0) return max(l, r) + 1; // 要找的点是左右子树中的一个，直接path + 1
            } else {
                // 是要找的点, 如果有一个在左右子树，res就是左右子树，返回自身的高度。
                // 如果答案是左右子树，浅层递归不会走到这边了。否则就是返回当前的深度1，即当前节点是所要找的值，且左右子树都是0
                res = max(l, r);
                return 1;
            }
            return 0;
        };
        dfs(root);
        return res;
    }
};