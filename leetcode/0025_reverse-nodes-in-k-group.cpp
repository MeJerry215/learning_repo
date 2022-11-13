#include "common.h"


/*
给你链表的头节点 head ，每 k 个节点一组进行翻转，请你返回修改后的链表。
k 是一个正整数，它的值小于或等于链表的长度。如果节点总数不是 k 的整数倍，那么请将最后剩余的节点保持原有顺序。
你不能只是单纯的改变节点内部的值，而是需要实际进行节点交换。

示例 1：
输入：head = [1,2,3,4,5], k = 2
输出：[2,1,4,3,5]
示例 2：

输入：head = [1,2,3,4,5], k = 3
输出：[3,2,1,4,5]
 
提示：
链表中的节点数目为 n
1 <= k <= n <= 5000
0 <= Node.val <= 1000
 

进阶：你可以设计一个只用 O(1) 额外内存空间的算法解决此问题吗？

通过次数413,458提交次数610,757

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/reverse-nodes-in-k-group
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/


class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode* dummy = new ListNode(0);
        dummy->next = head;
        ListNode* fast = dummy, *cur = dummy;
        for(int i = 0; i < k; i++) {
            fast = fast->next;
        }
        while(fast) {
            ListNode* kth_node = fast->next, *tail = cur->next, *tmp = cur->next->next;
            for(int i = 0; i < k - 1; i++) {
                if (fast) fast = fast->next;
                ListNode* next = tmp->next;
                tmp->next = cur->next;
                cur->next = tmp;
                tmp = next;
            }
            if (fast) fast = fast->next;
            tail->next = tmp;
            if (!fast) tail->next = kth_node;
            cur = tail;
        }
        return dummy->next;
    }
};

void TestSolution() {
    Solution s;
    ListNode* head = make_list_node({1,2,3,4,5, 6});
    s.reverseKGroup(head, 2);
}