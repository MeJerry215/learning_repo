#include "common.h"

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution
{
public:
    TreeNode *bstFromPreorderHelper(vector<int> &preorder, int l, int r) {
        if (l > r) {
            return nullptr;
        }
        TreeNode *d = new TreeNode(preorder[l]);
        if (l == r)
            return d;

        int left = l, right = r;
        while (left < right) {
            int mid = left + (right - left + 1) / 2;
            if (preorder[mid] < preorder[l]) {
                left = mid;
            } else {
                // 下一轮搜索区间是 [l, mid - 1]
                right = mid - 1;
            }
        }

        d->left = bstFromPreorderHelper(preorder, l + 1, left);
        d->right = bstFromPreorderHelper(preorder, left + 1, r);
        return d;
    }

    TreeNode *bstFromPreorder(vector<int> &preorder) {
        return bstFromPreorderHelper(preorder, 0, preorder.size() - 1);
    }
};