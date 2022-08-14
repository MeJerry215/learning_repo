#include "common.h"

class Solution {
public:
    ListNode* mergeInBetween(ListNode* list1, int a, int b, ListNode* list2) {
        ListNode* new_head = new ListNode(0);
        new_head->next = list1;
        ListNode* pre = new_head;
        ListNode* tail = nullptr;
        int i = 0;
        for(; i < a; i++) {
            pre = pre->next;
        }
        tail = pre;
        for(; i < b; i++) {
            tail = tail->next;
        }
        pre->next = tail->next;
        while(tail->next) {
            tail = tail->next;
        }
        tail->next = list2;
        return new_head->next;
    }
};