#include "common.h"

class Solution
{
public:
    vector<int> largestValues(TreeNode* root) {
        vector<int> res;
        function<void(TreeNode*, int)> dfs = [&](TreeNode* node, int depth) {
            if (!node) return;
            if (res.size() == depth) {
                res.push_back(node->val);
            }
            dfs(node->left, depth + 1);
            dfs(node->right, depth + 1);
            res[depth] = max(res[depth], node->val);
        };
        dfs(root, 0);
        return res;
    }
};