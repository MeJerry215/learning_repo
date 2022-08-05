#include "common.h"

class Solution {
public:

    TreeNode* constructMaximumBinaryTreeHelper(vector<int>& nums, int low , int high) {
        if (low > high) {
            return nullptr;
        }
        TreeNode *root = new TreeNode(nums[high]);
        int mid = (low + high - 1) / 2;
        root->left = constructMaximumBinaryTreeHelper(nums, low, mid);
        root->right = constructMaximumBinaryTreeHelper(nums, mid + 1, high - 1);
        return root;
    } 

    TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        TreeNode* root = nullptr;
        int size = nums.size();
        if (size > 0) {
            root = constructMaximumBinaryTreeHelper(nums, 0, size - 1);
        }
        
// [0, 1, 2, 3, 5, 6]
        return root;
    }
};