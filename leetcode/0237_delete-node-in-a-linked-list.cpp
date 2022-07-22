#include "common.h"

class Solution {
public:
    void deleteNode(ListNode* node) {
        if (!node) return;
        node->val = node->next->val;
        node->next = node->next->next
    }
};