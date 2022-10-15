#include "common.h"


class Solution {
public:
    struct compare {
        bool operator()(const ListNode* lhs, const ListNode* rhs) {
            return lhs->val > rhs->val;
        }
    };


    ListNode* mergeKLists(vector<ListNode*>& lists) {
        ListNode *head = new ListNode(0);
        ListNode *cur = head;
        priority_queue<ListNode*, vector<ListNode*>, compare> merge_lists;
        for(ListNode* node: lists) {
            if (node)
                merge_lists.push(node);
        }
        while(merge_lists.size()) {
            ListNode *node = merge_lists.top();
            merge_lists.pop();
            cur->next = node;
            cur = node;
            node = node->next;
            if (node) {
                merge_lists.push(node);
            }
            cur->next = nullptr;
        }

        return head->next;
    }
};