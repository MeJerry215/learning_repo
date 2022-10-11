#include "common.h"

class Solution
{
public:
    vector<string> binaryTreePaths(TreeNode *root)
    {
        vector<string> res;
        function<void(TreeNode *, string)> dfs = [&](TreeNode *node, string path)
        {
            if (!node)
                return;
            
            if (path.size())
            {
                path = path + "->" + to_string(node->val);
            }
            else
            {
                path = to_string(node->val);
            }
            if (!node->left && !node->right)
            {
                res.push_back(path);
            }
            dfs(node->left, path);
            dfs(node->right, path);
        };
        string path_str = "";
        dfs(root, path_str);
        return res;
    }
};