#include "common.h"

class Solution {
public:
    int ans;

    int depth(TreeNode *node) {
        if (!node) {
            return 0;
        }
        // 左树高度
        int l = depth(node->left);
        // 右树高度
        int r = depth(node->right);
        // 左右树节点相加, 加上本节点个数
        ans = max(ans, l + r + 1);
        return max(l, r) + 1;
    }

    int diameterOfBinaryTree(TreeNode* root) {
        ans = 1;
        depth(root);
        return ans - 1;
    }
};
