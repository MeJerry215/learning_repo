#include "common.h"

class Solution
{
public:
    ListNode *reverseBetween(ListNode *head, int left, int right)
    {
        ListNode *tmp = new ListNode(INT_MAX);
        tmp->next = head;
        ListNode *l1 = head;
        ListNode *l2 = head;
        bool is_change = true;
        function<void(ListNode *&, ListNode *)> dfs = [&](ListNode *&lhs, ListNode *rhs)
        {
            if (rhs->next->val != right)
            {
                if (lhs->next->val != left)
                {
                    dfs(lhs->next, rhs->next);
                }
                else
                {
                    dfs(lhs, rhs->next);
                }
            }
            if (is_change)
            {
                if (lhs == rhs) {
                    is_change = false;
                    return;
                } 
                ListNode *tmp = lhs->next;
                lhs->next = rhs->next;
                rhs->next = tmp;
                tmp = lhs->next->next;
                lhs->next->next = rhs->next->next;
                rhs->next->next = tmp;
                if (lhs->next == rhs) {
                    is_change = false;
                }
            }
        };
        dfs(l1, l2);
        return tmp->next;
    }
};