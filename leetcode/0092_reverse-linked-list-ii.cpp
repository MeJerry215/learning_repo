#include "common.h"


class Solution {
public:
    ListNode *reverseBetween(ListNode *head, int left, int right) {
        ListNode *dummyNode = new ListNode(-1);
        dummyNode->next = head;
        ListNode *pre = dummyNode;
        for (int i = 0; i < left - 1; i++) {
            pre = pre->next;
        }
        ListNode *cur = pre->next;
        ListNode *next;
        for (int i = 0; i < right - left; i++) {
            next = cur->next;
            cur->next = next->next;
            next->next = pre->next;
            pre->next = next;
        }
        return dummyNode->next;
    }

};

void TestSolution() {
    Solution s;
    ListNode* head = make_list_node({1, 2, 3, 4, 5});
    auto res = s.reverseBetween(head, 2, 3);
    print_list_node(res);
}