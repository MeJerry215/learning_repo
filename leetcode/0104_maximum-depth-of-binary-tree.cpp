#include "common.h"


class Solution {
public:

    int maxDepthHelper(TreeNode* root, int depth) {
        if (!root) {
            return depth;
        }
        int l_max = maxDepthHelper(root->left, depth + 1);
        int r_max = maxDepthHelper(root->right, depth + 1);
        return max(l_max, r_max);
    }

    int maxDepth(TreeNode* root) {
        return maxDepthHelper(root, 0);
    }
};

void TestSolution() {
    Solution s;
    TreeNode* root = make_tree_node({1, 2, 3, INT_MIN, 4, 5, 6});
    cout << s.maxDepth(root) << endl;
}