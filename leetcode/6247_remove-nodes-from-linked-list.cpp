#include "common.h"


class Solution {
public:
    ListNode* removeNodes(ListNode* head) {
        ListNode* new_head = new ListNode(INT_MAX);
        new_head->next = head;
        vector<ListNode*> desc_stk;
        ListNode* cur = new_head;
        while(cur) {
            while(!desc_stk.empty() && desc_stk.back()->val < cur->val) {
                desc_stk.pop_back();
            }
            desc_stk.push_back(cur);
            cur = cur->next;
        }
        for(int i = 0; i < desc_stk.size() - 1; i++) {
            desc_stk[i]->next = desc_stk[i + 1];
        }
        return new_head->next;
    }


    ListNode* removeNodes(ListNode* head) {
        if (!head->next) return head;
        ListNode* max_node = removeNodes(head->next);
        if (head->val < max_node->val) return max_node;
        head->next = max_node;
        return head;
    }
};