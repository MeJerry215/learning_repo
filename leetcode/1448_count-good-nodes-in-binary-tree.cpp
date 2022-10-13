#include "common.h"

/*
Given a binary tree root, a node X in the tree is named good if in the path from root to X there are no nodes with a value greater than X.
Return the number of good nodes in the binary tree.

Example 1:
Input: root = [3,1,4,3,null,1,5]
Output: 4
Explanation: Nodes in blue are good.
Root Node (3) is always a good node.
Node 4 -> (3,4) is the maximum value in the path starting from the root.
Node 5 -> (3,4,5) is the maximum value in the path
Node 3 -> (3,1,3) is the maximum value in the path.
Example 2:
Input: root = [3,3,null,4,2]
Output: 3
Explanation: Node 2 -> (3, 3, 2) is not good, because "3" is higher than it.
Example 3:
Input: root = [1]
Output: 1
Explanation: Root is considered as good.
Constraints:

The number of nodes in the binary tree is in the range [1, 10^5].
Each node's value is between [-10^4, 10^4].

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/count-good-nodes-in-binary-tree
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution
{
public:
  int res = 0;

  void goodNodesHelper(TreeNode *node, int max_val)
  {
    if (!node)
      return;
    if (max_val <= node->val)
      res++;
    max_val = max(node->val, max_val);
    goodNodesHelper(node->left, max_val);
    goodNodesHelper(node->right, max_val);
  }

  int goodNodes(TreeNode *root)
  {
    if (!root)
      return 0;
    goodNodesHelper(root, root->val);
    return res;
  }
};