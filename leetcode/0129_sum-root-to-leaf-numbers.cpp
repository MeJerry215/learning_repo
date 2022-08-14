#include "common.h"

class Solution {
public:
    int ans = 0;

    void sumNumbersHelper(TreeNode* root, string& nums) {
        if (!root) {
            return;
        }
        nums += to_string(root->val);
        if (!root->left && !root->right) {
            ans += atoi(nums.c_str());
            nums.pop_back();
            return;
        }
        sumNumbersHelper(root->left, nums);
        sumNumbersHelper(root->right, nums);

        nums.pop_back();
    }

    int sumNumbers(TreeNode* root) {
        string nums = "";
        sumNumbersHelper(root, nums);
        return ans;
    }
};