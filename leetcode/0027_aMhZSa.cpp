#include "common.h"

class Solution {
public:
    bool isPalindrome(ListNode* head) {
        bool res = true;
        ListNode* pre = head;
        function<void(ListNode*)> dfs = [&](ListNode* node) {
            if (!node) return;
            dfs(node->next);
            if (node->val != pre->val) res = false;
            pre = pre->next;
        };
        dfs(head);
        return res;
    }
};