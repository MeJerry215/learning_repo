#include "common.h"

class Solution {
public:
    vector<TreeNode*> delNodes(TreeNode* root, vector<int>& to_delete) {
        vector<TreeNode*> res;
        unordered_set<int> delete_node;
        for(auto num: to_delete) {
            delete_node.insert(num);
        }
        function<TreeNode*(TreeNode*, bool)> dfs = [&](TreeNode* node, bool parent_delete) -> TreeNode* {
            if (!node) return nullptr;
            if (delete_node.count(node->val)) {
                dfs(node->left, true);
                dfs(node->right, true);
                return nullptr;
            } else {
                if (parent_delete) {
                    res.push_back(node);
                }
                node->left = dfs(node->left, false);
                node->right = dfs(node->right, false);
                return node;
            }
        };
        dfs(root, true);
        return res;
    }
};