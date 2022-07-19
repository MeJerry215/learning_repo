#include "common.h"


class Solution {
public:
    ListNode* find_middle_node(ListNode* head, ListNode* tail) {
        ListNode* fast = head;
        while(fast != tail && fast->next != tail) {
            fast = fast->next->next;
            head = head->next;
        }
        return head;
    }

    TreeNode* build_BST(ListNode *head, ListNode* tail) {
        if (head == tail) return nullptr;
        ListNode* middle = find_middle_node(head, tail);
        TreeNode* root = new TreeNode(middle->val);
        root->left = build_BST(head, middle);
        root->right = build_BST(middle->next, tail);
        return root;
    }

    TreeNode* sortedListToBST2(ListNode* head) {
        TreeNode* root = build_BST(head, nullptr);
        return root;
    }


/*
假设一个6个节点的平衡二叉树进行中序遍历
                        2
                    /       \
                   1        4
                /        /    \ 
              0         3      5
则按照平衡二叉树的中序遍历顺序就是 生成的链表
对于左右子树 可以换分为 [left, mid - 1] mid [mid + 1, right]
当 left > mid - 1时候，则无节点
*/
    TreeNode* divide_build_BST(ListNode*& head, int left , int right) {
        if (left > right) return nullptr;
        TreeNode *root = new TreeNode(0);
        int mid = (left + right) / 2;
        root->left = divide_build_BST(head, left, mid - 1);
        root->val = head->val;
        head = head->next;
        root ->right = divide_build_BST(head, mid + 1, right);
        return root;
    }

    int get_length(ListNode* head) {
        int i = 0;
        while(head) {
            i ++;
            head = head->next;
        }
        return i;
    }

    TreeNode* sortedListToBST(ListNode* head) {
        int n = get_length(head);
        return divide_build_BST(head, 0, n - 1);
    }


};