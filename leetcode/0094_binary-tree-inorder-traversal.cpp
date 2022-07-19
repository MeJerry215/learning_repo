#include "common.h"


class Solution {
public:
    void inorderTraversal_helper(TreeNode* root, vector<int>& ans) {
        if (root == nullptr) return;
        ans.push_back(root->val);
        inorderTraversal_helper(root->left, ans);
        inorderTraversal_helper(root->right, ans);
    }

    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> ans;
        inorderTraversal_helper(root, ans);
        return ans;
    }

    vector<int> preorderTraversal(TreeNode* root) {
        vector<TreeNode*> node_stack;
        vector<int> ans;
         TreeNode* cur = root;
         while(cur || !node_stack.empty()){
             while(cur){
                ans.push_back(cur->val);
                node_stack.push_back(cur);
                cur = cur->left;
             }
             cur = node_stack.back();
             node_stack.pop_back();
             cur = cur->right;
         }
         return ans; 

    }

    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> ans;
        vector<TreeNode*> node_stack;
        TreeNode* cur = root;
        while(cur || !node_stack.empty()){
            while(cur){
                node_stack.push_back(cur);
                cur = cur->left;
            }
            cur = node_stack.back();
            node_stack.pop_back();
            ans.push_back(cur->val);
            cur = cur->right;
        }
        return ans;
    }

    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> ans;
        vector<TreeNode*> node_stack;
        TreeNode *pre = nullptr;
        while(root || !node_stack.empty()) {
            while(root) {
                node_stack.push_back(root);
                root = root->left;
            }
            root = node_stack.back();
            if (root->right == nullptr || root->right == pre) {
                ans.push_back(root->val);
                node_stack.pop_back();
                pre = root;
                root = nullptr;
            } else {
                root = root->right;
            }
        }
        return ans;
    }
};