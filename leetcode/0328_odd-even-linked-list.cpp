#include "common.h"

class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        ListNode* odd = new ListNode(0);
        ListNode* even = new ListNode(0);
        ListNode* oddTmp = odd;
        ListNode* evenTmp = even;
        int i = 0;
        while(head) {
            ListNode* nextTmp = head->next;
            if (i++ % 2 == 0) {
                oddTmp->next = head;
                oddTmp = oddTmp->next;
                oddTmp->next = nullptr;
            } else {
                evenTmp->next = head;
                evenTmp = evenTmp->next;
                evenTmp->next = nullptr;
            }
            head = nextTmp;
        }
        oddTmp->next = even->next;
        return odd->next;
    }
};