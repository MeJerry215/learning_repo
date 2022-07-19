#include "common.h"


/*
假设slow fast 在 k个点相遇, 则慢指针走了k步, 快指针走了2k步。
无环部分为n, 有环部分m
n + m = k
slow 每次走一步，fast每次走两步，则追击速度为1。则必然追及到。
fast 走的长度 为 k已经走的，以及圈中剩余的j, 以及圈中的m则有
2k = m + n + k + m
m + n = k
则k = n
这个时候我们要到环的起始位置 就是要走n步，而fast指针再走n步也到了环的起始位置
则 fast 和head 共同前进n步就相交了
*/
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {
        if (!head) return nullptr;
        ListNode *slow, *fast;
        slow = fast = head;
        while(fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) break;
        }
        if (!slow || !fast) return nullptr;
        slow = head;
        while(slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }
        return slow;
    }
};

