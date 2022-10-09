#include "common.h"

/*
给定一棵二叉树的根节点 root ，请找出该二叉树中每一层的最大值。

 

示例1：

输入: root = [1,3,2,5,3,null,9]
输出: [1,3,9]
解释:
          1
         / \
        3   2
       / \   \  
      5   3   9 
示例2：

输入: root = [1,2,3]
输出: [1,3]
解释:
          1
         / \
        2   3
示例3：

输入: root = [1]
输出: [1]
示例4：

输入: root = [1,null,2]
输出: [1,2]
解释:      
           1 
            \
             2     
示例5：

输入: root = []
输出: []
 

提示：

二叉树的节点个数的范围是 [0,104]
-231 <= Node.val <= 231 - 1
 

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/hPov7L
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。*
*/


/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<int> largestValues(TreeNode* root) {
    queue<TreeNode*> nodes;
    nodes.push(root);
    vector<int> res;
    while (nodes.size()) {
      int n = nodes.size();
      int max_val = INT_MIN;
      for (int i = 0; i < n; i++) {
        TreeNode *node = nodes.front();
        nodes.pop();
        if (node->left)
          nodes.push(node->left);
        if (node->right)
        nodes.push(node->right);
        if (node->val > max_val) max_val = node->val;
      }
      res.push_back(max_val);
    }
    return res;
  }
};