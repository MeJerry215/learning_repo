#include "common.h"

/*
给你一棵二叉树的根节点 root ，返回树的 最大宽度 。
树的 最大宽度 是所有层中最大的 宽度 。
每一层的 宽度 被定义为该层最左和最右的非空节点（即，两个端点）之间的长度。将这个二叉树视作与满二叉树结构相同，两端点间会出现一些延伸到这一层的 null 节点，这些 null 节点也计入长度。
题目数据保证答案将会在  32 位 带符号整数范围内。

示例 1：
输入：root = [1,3,2,5,3,null,9]
输出：4
解释：最大宽度出现在树的第 3 层，宽度为 4 (5,3,null,9) 。
示例 2：

输入：root = [1,3,2,5,null,null,9,6,null,7]
输出：7
解释：最大宽度出现在树的第 4 层，宽度为 7 (6,null,null,null,null,null,7) 。
示例 3：

输入：root = [1,3,2,5]
输出：2
解释：最大宽度出现在树的第 2 层，宽度为 2 (3,2) 。
 

提示：

树中节点的数目范围是 [1, 3000]
-100 <= Node.val <= 100

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/maximum-width-of-binary-tree
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    int res = 0;

    void dfsWidthOfBinaryTree(TreeNode* node, long long index, int depth, vector<int>& leftmost_index) {
        if (!node) return;
        if (leftmost_index.size() == depth) {
            leftmost_index.push_back(index);
        }
        index %= ((long long)1 << 62);
        dfsWidthOfBinaryTree(node->left, index * 2, depth + 1, leftmost_index);
        dfsWidthOfBinaryTree(node->right, index * 2 + 1, depth + 1, leftmost_index);
        res = max(res, int(index - leftmost_index[depth] + 1));
    }

    int widthOfBinaryTree(TreeNode* root) {
        /*
        树中的节点，如果是满树的话 是 2^n - 1 n为树的层数，所以
        对于树中每个节点都可以进行编号，如果根节点为1, 则左右子，左子树为 2 右子树为 3 通用归纳为 n , 2n, 2n + 1
                                1
                        /               \
                      2                  3
                /           \       /           \
                4           5       6           7
                        /      \             /      \
                      10      11           14       15
        根据以上特性只要辅助空间 数的深度的 表示树的最左节点即可。 
        针对以上子树 则 产生 [1, 2, 4, 10]的辅助数组，而当遍历到任意节点时候，根据深度减去辅助数组中元素最小值 然后加1 则就是树的最大宽度
        */
        if (!root) return 0;
        res = 1;
        vector<int> leftmost_index;
        dfsWidthOfBinaryTree(root, 1, 0, leftmost_index);
        return res;
    }
};