#include "common.h"

class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node() : val(0), left(NULL), right(NULL), next(NULL) {}

    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}

    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};


class Solution {
public:
    Node* connect(Node* root) {
        if (root == nullptr) {
            return root;
        }
        Node* cur = root;
        Node* dummy = new Node(0);
        while(cur != nullptr) {
            dummy->next = nullptr;
            Node* pre = dummy;
            while(cur) {
                if (cur->left != nullptr) {
                    pre->next = cur->left;
                    pre = pre->next;
                }
                if (cur->right != nullptr) {
                    pre->next = cur->right;
                    pre = pre->next;
                }
                cur = cur->next;
            }
            cur = dummy->next;
        }
        return root;
    }
};