#include "common.h"

/*
给你一个链表，删除链表的倒数第 n 个结点，并且返回链表的头结点。
示例 1：

输入：head = [1,2,3,4,5], n = 2
输出：[1,2,3,5]
示例 2：
输入：head = [1], n = 1
输出：[]
示例 3：
输入：head = [1,2], n = 1
输出：[1]

提示：
链表中结点的数目为 sz
1 <= sz <= 30
0 <= Node.val <= 100
1 <= n <= sz
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/remove-nth-node-from-end-of-list
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/


class Solution {
public:
    ListNode* removeNthFromEnd2(ListNode* head, int n) {
        ListNode* new_head = new ListNode(0);
        new_head->next = head;
        ListNode *before = new_head;
        ListNode* cur = new_head;
        int i = 0;
        while(cur) {
            cur = cur->next;
            i += 1;
            if (i > n) {
                before = before->next;
            }
        }
        before->next = before->next->next;

        return new_head->next;
    }

    int cur=0;
    ListNode* removeNthFromEnd(ListNode* head, int n) {
       if(!head) return NULL;
       head->next = removeNthFromEnd(head->next,n);
       cur++;
       if(n==cur) return head->next;
       return head;
    }
};