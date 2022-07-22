#include "common.h"

class Solution {
public:

    ListNode* pre = nullptr;
    bool ans = true;

    ListNode* isPalindrome_helper(ListNode* head) {
        if (!head) return nullptr;
        head->next = isPalindrome_helper(head->next);
        if(pre->val != head->val) {
            ans = false;
        } 
        pre = pre->next;
        return head;
    }


    bool isPalindrome(ListNode* head) {
        pre = head;
    }
};

void TestSolution() {
    Solution s;
    vector<int> arr = {1, 2, 3, 4};
    ListNode* head =  make_list_node(arr);
    head = s.isPalindrome_helper(head);
    print_list_node(head);
}