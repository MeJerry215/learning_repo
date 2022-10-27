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
    TreeNode *sortedArrayToBST(vector<int> &nums)
    {
        function<TreeNode *(int, int)> dfs = [&](int l, int r) -> TreeNode *
        {
            if (l > r)
                return nullptr;
            if (l == r)
            {
                return new TreeNode(nums[l]);
            }
            int mid = (l + r + 1) / 2;
            TreeNode *left = dfs(l, mid - 1);
            TreeNode *right = dfs(mid + 1, r);
            TreeNode *root = new TreeNode(nums[mid]);
            root->left = left;
            root->right = right;
            return root;
        };
        TreeNode *root = dfs(0, nums.size() - 1);
        return root;
    }
};