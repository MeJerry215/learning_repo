#include "common.h"

class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode* new_head = new ListNode(INT_MIN);
        new_head->next = head;
        ListNode* cur = new_head;
        while(cur->next && cur->next->next) {
            if (cur->next->val == cur->next->next->val) {
                ListNode* tail = cur->next->next->next;
                while(tail && tail->val == cur->next->val) {
                    tail = tail->next;
                }
                cur->next = tail;
            } else {
                cur = cur->next;
            }
        }
        return new_head->next;
    }
};