#include "common.h"

class Solution
{
public:

    int averageOfSubtreeHelper(TreeNode* root, int& sum, int& n)
    {
        if (root == nullptr) {
            sum = n = 0;
            return 0;
        }

        int lsum, rsum, ln, rn;
        int lres = averageOfSubtreeHelper(root->left, lsum, ln);
        int rres = averageOfSubtreeHelper(root->right, rsum, rn);
        sum = lsum + rsum + root->val;
        n = ln + rn + 1;
        return sum / n * n == root->val * n ? lres + rres + 1 : lres + rres;
    }

    int averageOfSubtree(TreeNode* root)
    {
        int sum = 0, n = 0;
        return averageOfSubtreeHelper(root, sum, n);
    }
};


void TestSolution()
{
    Solution s;
    TreeNode* root = make_tree_node({4, 8, 5, 0, 1, INT_MIN, 6});
    // bfs_tree_node(root);
    std::cout << s.averageOfSubtree(root) << std::endl;
}