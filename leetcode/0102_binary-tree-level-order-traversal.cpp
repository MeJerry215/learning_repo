#include "common.h"


class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        queue<TreeNode*> node_queue;
        if (root) {
            node_queue.push(root);
        }
        vector<vector<int>> ans;
        while(node_queue.size() > 0) {
            int size = node_queue.size();
            vector<int> layer;
            for(int i = 0; i < size; i++) {
                TreeNode* node = node_queue.front();
                node_queue.pop();
                layer.push_back(node->val);
                if (node->left) node_queue.push(node->left);
                if (node->right) node_queue.push(node->right);
            }
            ans.push_back(layer);
        }
        return ans;
    }
};