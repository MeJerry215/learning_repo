#include "common.h"




class Node {
public:
    int val;
    vector<Node*> children;

    Node() {}

    Node(int _val) {
        val = _val;
    }

    Node(int _val, vector<Node*> _children) {
        val = _val;
        children = _children;
    }
};

class Solution {
public:
    vector<int> postorder(Node* root) {
        vector<int> res;
        function<void(Node*)> dfs = [&](Node* node) -> void {
            if (!node) {
                return;
            }
            for(auto child: node->children) {
                dfs(child);
            }
            res.push_back(node->val);
        };
        dfs(root);
        return res;
    }
};