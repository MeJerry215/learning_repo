#include "common.h"

class Solution {
public:
    int get_length(ListNode* head) {
        int i = 0;
        while(head) {
            i++;
            head = head->next;
        }
        return i;
    }


    ListNode* rotateRight(ListNode* head, int k) {
        int len = get_length(head);
        if (!len) return head;
        k = k % len;
        ListNode* slow = head;
        ListNode* fast = head;
        for(int i = 0; i < k; i++) {
            fast = fast->next;
        }
        while(fast->next) {
            fast = fast->next;
            slow = slow->next;
        }
        fast->next = head;
        fast = slow->next;
        slow->next = nullptr;
        return fast;
    }
};


void TestSolution() {
    vector<int> arr{1, 2, 3, 4, 5, 6};
    ListNode* l = make_list_node(arr);
    Solution s;
    ListNode* ans = s.rotateRight(l, 3);
    print_list_node(ans);
}