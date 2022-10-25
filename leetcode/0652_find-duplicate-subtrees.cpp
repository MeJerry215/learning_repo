#include "common.h"


constexpr size_t offset = 14695981039346656037ULL, prime = 1099511628211ULL;
class Solution {
public:
    uint32_t knuth(int x, int p=30) {
    const uint32_t knuth = 2654435769;
    const uint32_t y = x;
    return (y * knuth) >> (32 - p);
  }

  vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
    vector<TreeNode*> res;
    unordered_map<uint32_t, vector<TreeNode*>> hash_nodes;
    function<long long(TreeNode*)> dfs = [&](TreeNode *node) -> uint32_t {
      if (!node) return 0;
      uint32_t hash_l = dfs(node->left);
      uint32_t hash_r = dfs(node->right);
      uint32_t hash_val = knuth(node->val) + hash_l + hash_r;
      hash_nodes[hash_val].push_back(node);
      return hash_val;
    };
    dfs(root);
    for(auto iter: hash_nodes) {
      cout << iter.first << " " << iter.second.size() << endl;
      if (iter.second.size() > 1) {
        res.push_back(iter.second[0]);
      }
    }
    return res;
  }
};