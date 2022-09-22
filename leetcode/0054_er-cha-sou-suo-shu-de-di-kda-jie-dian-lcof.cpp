#include "common.h"

/**
给定一棵二叉搜索树，请找出其中第 k 大的节点的值。
示例 1:

输入: root = [3,1,4,null,2], k = 1
   3
  / \
 1   4
  \
   2
输出: 4
示例 2:

输入: root = [5,3,6,2,4,null,null,1], k = 3
       5
      / \
     3   6
    / \
   2   4
  /
 1
输出: 4

限制：
1 ≤ k ≤ 二叉搜索树元素个数

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/er-cha-sou-suo-shu-de-di-kda-jie-dian-lcof
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
 */
class Solution {
public:
    int kthLargest(TreeNode* root, int k) {
        int i = 0;
        int res = 0;
        function<void(TreeNode*)> rvl = [&](TreeNode* node) {
            if (!node) {
                return;
            }
            if (i >= k) return;
            rvl(node->right);
            i++;
            if (i == k) res = node->val;
            rvl(node->left);
        };
        rvl(root);
        return res;
    }
};