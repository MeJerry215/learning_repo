#include "common.h"

class Solution {
public:
    int minDepth(TreeNode* root) {
        if(!root)   return 0;
        if(!root->left || !root->right)    
            return max(minDepth(root->left)+1, minDepth(root->right)+1);
        return min(minDepth(root->left)+1, minDepth(root->right)+1);
    }
};

void TestSolution() {
//  cmake --build . --target INSTALL -j 32
}