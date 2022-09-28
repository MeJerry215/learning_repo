#include "common.h"

/*
给你二叉树的根节点 root ，返回其节点值的 锯齿形层序遍历
。（即先从左往右，再从右往左进行下一层遍历，以此类推，层与层之间交替进行）。

 

示例 1：


输入：root = [3,9,20,null,null,15,7]
输出：[[3],[20,9],[15,7]]
示例 2：

输入：root = [1]
输出：[[1]]
示例 3：

输入：root = []
输出：[]
 

提示：

树中节点数目在范围 [0, 2000] 内
-100 <= Node.val <= 100

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/binary-tree-zigzag-level-order-traversal
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution
{
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode *root) {
        vector<vector<int>> res;
        // 这里可以考虑用双端队列, 这样就不用reverse了
        queue<TreeNode *> to_visit_node;
        if (root) {
            to_visit_node.push(root);
            to_visit_node.push(nullptr);
        }
        vector<int> layer;
        while (to_visit_node.size() > 0) {
            TreeNode *node = to_visit_node.front();
            to_visit_node.pop();
            if (!node) {
                if (res.size() % 2) {
                    reverse(layer.begin(), layer.end());
                }
                res.push_back(layer);
                layer.clear();
                if (to_visit_node.size())
                    to_visit_node.push(nullptr);
                continue;
            };
            layer.push_back(node->val);
            if (node->left)
                to_visit_node.push(node->left);
            if (node->right)
                to_visit_node.push(node->right);
        }

        return res;
    }
};