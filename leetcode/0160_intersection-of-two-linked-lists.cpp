#include "common.h"

class Solution {
public:

    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        ListNode* p1 = headA;
        ListNode* p2 = headB;
        while(p1 || p2) {
            if (!p1) p1 = headB;
            if (!p2) p2 = headA;
            if (p1 == p2) return p1;
            p1 = p1->next;
            p2 = p2->next;
        }
        return nullptr
    }
};