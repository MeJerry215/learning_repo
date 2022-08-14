#include "common.h"

class Solution {
public:
    int len = 0;
    

    ListNode* dfs_middleNodel_helper(ListNode* node, int n) {
        if (!node) {
            len = n - 1;
            return nullptr;
        }
        auto ret = dfs_middleNodel_helper(node->next, n + 1);
        if ((len + 1) / 2 == n) {
            return node;
        }
        return ret  ? ret : nullptr;
    }

    ListNode* middleNode2(ListNode* head) {
        return dfs_middleNodel_helper(head, 0);
    }

    ListNode* middleNode(ListNode* head) {
        ListNode* fast = head;
        while(fast && fast->next) {
            head = head->next;
            fast = fast->next->next;
        }
        return fast ? head->next: head;
    }
};