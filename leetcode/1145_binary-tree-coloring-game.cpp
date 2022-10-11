#include "common.h"

/*
 * @lc app=leetcode.cn id=1145 lang=cpp
 *
 * [1145] 二叉树着色游戏
 */


/*
            1
        /       \
      2           3
    /   \       /   \
   5    6      7    8
  / \   /
 9  10 11
 由于每次涂色选择一个他之前涂好颜色的节点的相邻结点，所以我们在选择利益化最大分支的时候首先第二步就是划地盘。
 所以如何划地盘，就是正面对抗，选择他选择的结点的临近节点。这样选择的同时也避免了一定的不确定性，
 可以直接根据当前遍历到的节点算出我可以得到的最大节点数目。
 根据3的子节点和父节点，我们可以选择1, 7, 8。 实际上我们可以选择必胜的点就是1。
 只要我们选择了父节点1，red就无法吃下我们剩下的子，所以我觉得例子中给的参考举例选择2并不是最好，可能带来一定的迷惑性。
 我们的最优选择点只能发生在red 已选节点的子节点和父节点。
 所以只要计算一下我们选择子节点或者父节点是否能够比red多即可。
 使用L(node) 来表示子树节点数目，根据以上分析结果。
 如果我们选择了父节点，那么我们可以获得的最多节点数为 n - L(node), red 可以获得L(node)节点
 如果我们选择子节点，则我们可以获得最多节点数为 max(L(node->left), L(node->right)). red可以获得 L - max(L(node->left), L(node->right))节点
*/
class Solution
{
public:
    bool btreeGameWinningMove(TreeNode *root, int n, int x)
    {
        bool res = false;
        function<int(TreeNode *)> dfs = [&](TreeNode *node)
        {
            if (!node)
                return 0;
            int left_tree_node_num = dfs(node->left);
            int right_ree_node_num = dfs(node->right);
            int sub_tree_total_num = left_tree_node_num + right_ree_node_num + 1;
            if (node->val == x)
            {
                if (n - sub_tree_total_num > sub_tree_total_num)
                    res = true;
                if (max(left_tree_node_num, right_ree_node_num) > n - max(left_tree_node_num, right_ree_node_num))
                    res = true;
            }
            return sub_tree_total_num;
        };
        dfs(root);
        return res;
    }
};
