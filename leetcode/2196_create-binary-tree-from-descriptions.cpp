#include "common.h"

class Solution
{
public:
    TreeNode *createBinaryTree(vector<vector<int>> &descriptions)
    {
        unordered_map<int, TreeNode *> node_map;
        unordered_set<int> parents;
        for (auto &description : descriptions)
        {
            int parent = description[0];
            int child = description[1];
            bool is_left = description[2];
            parents.insert(parent);
            if (!node_map.count(parent))
            {
                TreeNode *parent_node = new TreeNode(parent);
                node_map[parent] = parent_node;
            }
            if (!node_map.count(child))
            {
                TreeNode *child_node = new TreeNode(child);
                node_map[child] = child_node;
            }
            TreeNode *parent_node = node_map[parent];
            TreeNode *child_node = node_map[child];
            if (is_left)
            {
                parent_node->left = child_node;
            }
            else
            {
                parent_node->right = child_node;
            }
        }
        for (auto &description : descriptions)
        {
            int child = description[1];
            if (parents.count(child))
            {
                parents.erase(child);
            }
        }
        return node_map[*parents.begin()];
    }
};