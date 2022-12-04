#include "common.h"


class Solution {
public:
    ListNode* deleteDuplicatesUnsorted(ListNode* head) 
    {
        // 注意，这里的重复元素要直接删除，所以只能先遍历一遍然后直接删除
        // 用hash表进行记录

        if( head->next == nullptr ) return head;

        // 设置哑节点，pre节点和探索节点
        ListNode* dummy = new ListNode(0),*pos = head;
        dummy->next = pos;
        ListNode* pre = dummy;

        // 找到重复元素，因为不能保留重复的元素，所以要先遍历然后直接删除
        unordered_map<int,int>  hashTable;
        while( head != nullptr )
        {
            hashTable[head->val]++;
            head = head->next;
        }

        while( pos != nullptr )
        {
            // 删除元素
            if( hashTable[pos->val] > 1)
            {
                pre->next = pos->next;
            }
            else
            {
                pre = pre->next;
            }

            pos = pos->next;
        }

        return dummy->next;
    }
};

