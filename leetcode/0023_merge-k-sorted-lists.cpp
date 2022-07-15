#include "common.h"


class Solution {
public:
    ListNode* mergeKLists_2(vector<ListNode*>& lists) {
        ListNode *head = new ListNode(0);
        ListNode *cur = head;
        while (lists.size() > 0)
        {
            ListNode *min_node = lists[0];
            int k = 0;
            if (min_node) {
            for(int i = 1; i < lists.size(); i++) {
                if (lists[i] && min_node->val > lists[i]->val) {
                    k = i;
                    min_node = lists[i];
                }
            }
            cur->next = min_node;
            min_node = min_node->next;
            cur = cur->next;
            cur->next = nullptr;
            lists[k] = min_node;

            }
            if (min_node == nullptr) {
                lists.erase(lists.begin() + k);
            }
        }
        
        return head->next;
    }

};