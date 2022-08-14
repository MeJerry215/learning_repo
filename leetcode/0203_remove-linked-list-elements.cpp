#include "common.h"

class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        ListNode* new_head = new ListNode(0);
        ListNode* cur = new_head;
        while(cur->next && cur->next->val == val) {
            cur = cur->next->next;
        }
        return new_head->next;
    }
};