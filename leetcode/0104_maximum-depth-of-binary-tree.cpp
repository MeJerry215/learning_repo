#include "common.h"


class Solution {
public:

    int maxDepth(TreeNode* root) {
        if (!root) return 0;
        return max(maxDepth(root->left), maxDepth(root->right)) + 1;
    }
};

void TestSolution() {
    Solution s;
    TreeNode* root = make_tree_node({1, 2, 3, INT_MIN, 4, 5, 6});
    cout << s.maxDepth(root) << endl;
}