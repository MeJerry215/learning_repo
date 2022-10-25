#include "common.h"


constexpr size_t offset = 14695981039346656037ULL, prime = 1099511628211ULL;
class Solution {
public:
    vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
        unordered_map<size_t, int> dict;
        vector<TreeNode*> ans;
        auto dfs = [&](auto& dfs, TreeNode* root) {
            if (!root) return offset;
            auto v = ((offset ^ dfs(dfs, root->left)) * prime ^ dfs(dfs, root->right)) * prime ^ size_t(root->val);
            if (dict[v]++ == 1) ans.push_back(root);
            return v;
        };
        dfs(dfs, root);
        return ans;
    }
};