#include "common.h"

class Solution {
public:

    ListNode* reverseList_v1(ListNode* head) {
        ListNode* cur = head, *pre = nullptr;
        while(cur) {
            ListNode* next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;            
        }
        return pre;
    }
    // 1 -> 2 -> 3 -> 4 -> null
    ListNode* reverseList(ListNode* head) {
        if (!head || !head->next) {
            return head;
        }
        ListNode* pre = reverseList(head->next);
        head->next->next = head;
        head->next = nullptr;
        return pre;
    }
};