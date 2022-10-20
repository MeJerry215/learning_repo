#include "common.h"

class Solution {
public:
    ListNode* mergeInBetween(ListNode* list1, int a, int b, ListNode* list2) {
        ListNode* new_head = new ListNode();
        new_head->next = list1;
        ListNode* cur = new_head;
        for(int i = 0; i < a; i++) {
          cur = cur->next;
        }
        for(int i = 0; i < (b - a + 1); i++) {
          cur->next = cur->next->next;
        }
        ListNode* list2_tail = list2;
        while(list2_tail && list2_tail->next) {
          list2_tail = list2_tail->next;
        }
        list2_tail->next = cur->next;
        cur->next = list2;
        return new_head->next;
  }
};