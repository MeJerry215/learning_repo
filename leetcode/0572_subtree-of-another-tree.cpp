#include "common.h"

class Solution
{
public:
    bool isSubTreeHelper(TreeNode *lhs, TreeNode *rhs) {
        // if left and right subtree both not exist they are the same
        if (!lhs && !rhs)
            return true;
        // if only one of them exists, they are not same
        if (!lhs || !rhs || lhs->val != rhs->val)
            return false;
        // the only circurstance is both them exists and they are the same
        // so the only thing to do is to make sure sub tree are the same.
        if (!isSubTreeHelper(lhs->left, rhs->left) || !isSubTreeHelper(lhs->right, rhs->right))
            return false;
        return true;
    }

    bool isSubtree(TreeNode *root, TreeNode *subRoot)
    {
        // only test the current node is same with subroot, if not same, test left and right tree.
        if (isSubTreeHelper(root, subRoot))
            return true;
        if (root && subRoot)
            return isSubtree(root->left, subRoot) || isSubtree(root->right, subRoot);
        return false;
    }
};