#include "common.h"

/*
给你一棵二叉树的根节点 root ，二叉树中节点的值 互不相同 。另给你一个整数 start 。在第 0 分钟，感染 将会从值为 start 的节点开始爆发。

每分钟，如果节点满足以下全部条件，就会被感染：

节点此前还没有感染。
节点与一个已感染节点相邻。
返回感染整棵树需要的分钟数。

 

示例 1：


输入：root = [1,5,3,null,4,10,6,9,2], start = 3
输出：4
解释：节点按以下过程被感染：
- 第 0 分钟：节点 3
- 第 1 分钟：节点 1、10、6
- 第 2 分钟：节点5
- 第 3 分钟：节点 4
- 第 4 分钟：节点 9 和 2
感染整棵树需要 4 分钟，所以返回 4 。
示例 2：


输入：root = [1], start = 1
输出：0
解释：第 0 分钟，树中唯一一个节点处于感染状态，返回 0 。
 

提示：

树中节点的数目在范围 [1, 105] 内
1 <= Node.val <= 105
每个节点的值 互不相同
树中必定存在值为 start 的节点

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/amount-of-time-for-binary-tree-to-be-infected
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
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
class Solution
{
    int res = 0;
    enum { INFECT_SIGN = 100000 };

public:
    int amountOfTimeHelper(TreeNode *node, int start) {
        if (!node)
            return 0;
        int ldepth = amountOfTimeHelper(node->left, start);
        int rdepth = amountOfTimeHelper(node->right, start);
        int max_depth = max(ldepth, rdepth);
        // no infectins, just return the depth.
        if (ldepth < INFECT_SIGN && rdepth < INFECT_SIGN && node->val != start) {
            return max(ldepth, rdepth) + 1;
        }
        // one of child has been infected, update the distance
        if (max_depth > INFECT_SIGN) {
            res = max(res, ldepth + rdepth - INFECT_SIGN);
        }
        // oops, the node itself has been infected. just caculate the max distance of to his child
        if (node->val == start) {
            res = max(res, max_depth);
            return INFECT_SIGN + 1;
        }
        return max_depth + 1;
    }

    /*
    this problem in other word say the max distance from start node.
    and the distance from farest node to start node will be the result.
                  1                             1
              /       \                       /   \
             5         3 +                   5     3
              \     /     \                      /   \
               4   10      6                    4     6 +
             /  \                              /       \
            9 -  2 -                          9         7
                                               \
                                                2 -
    in previous problems we know how to calculate the tree's max distance of any two node.
    in this situation, we assign one node and to find the farest node.
    normally we will return depth of a node the larger depth of it's two child tree,
    but if one of it's subtree has been effect we will use the effect node's depth.
    take above graph as an example, the node 3's left tree depth is 3, right tree is 2
    because the node 6 has been effect so the accutual depth will return 1.
    so we caculate the farest distance at node 3, the distance will 3 + 1 + 1 = 5
    if node 3 returns actual depth 4, node will get the wrong ans of 1 + 1 + 4 = 6.
    so only 3 returns the distance from node 6, 1 will get the real ans of 1 + 1 + 2 = 4.
    if a node his two child has no effected node, the caculate will make no sense.
    so we also need to know a node, which sub tree has been effected.
    if using + to annotate start point and - annoted end point
      \brief solve the problem by caculate the farest distance.
      \param root, root node
      \param start, start point to infect other node.
    */
    int amountOfTime(TreeNode *root, int start) {
        amountOfTimeHelper(root, start);
        return res;
    }
};