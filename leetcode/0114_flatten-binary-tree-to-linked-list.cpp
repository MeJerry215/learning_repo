#include "common.h"

/*
    the basic thinking of this solution is lrv, firstly flatten child, then flaten itself, as below example.
    if we want to flatten 2, it will flatten it's two child: 3 and 4 firstly, then flatten itself.
                    1                                                 1
                /     \         --- after flatten 3 --->           /    \
            2           8                                         2      8
          /   \           \                                      /  \      \
        3      4           9                                   3     4      9
       /      /  \                                              \   / \
      5      6    7                                              5  6  7

      --- after flatten 4 --->       1             --- after flattern 2 --->      1
                                  /      \                                     /     \
                                2         8                                   2       8
                             /     \        \                                  \      \
                            3       4        9                                  3      9
                             \       \                                           \
                              5       6                                           ...(5, 4, 6)
                                       \                                            \
                                        7                                            7
*/
class Solution
{
public:
    void flatten(TreeNode *root) {
        if (!root) return;
        // first to flatten child node
        flatten(root->left);
        flatten(root->right);
        /*
        ** after flatten child node find left rightmost child node, disconnect node with left child
        ** then make left child to be the node's right child, and the rightmost child node connect with nodes' origin child
        */
        TreeNode *l_child = root->left;
        TreeNode *r_child = root->right;
        TreeNode *l_rightmost = root;
        root->left = nullptr;
        // if left child exists, find the rightmost child, this child node will connect to right child, make tree flatten.
        if (l_child) {
            l_rightmost = l_child;
            while (l_rightmost->right) {
                l_rightmost = l_rightmost->right;
            }
            root->right = l_child;
        }
        l_rightmost->right = r_child;
    }
};