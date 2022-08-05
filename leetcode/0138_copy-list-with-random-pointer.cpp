#include "common.h"

class Node {
public:
    int val;
    Node* next;
    Node* random;
    
    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};

/**
 * 思路就是边走边拷贝, 同时带memo, 映射途中已经创建出来的节点，如果查询到创建，那么久使用创建出来的节点，没有就创建。
 */
class Solution {
public:
    Node* copyRandomList(Node* head) {
        if (head == nullptr) {
            return nullptr;
        }
        Node* new_head = new Node(0);
        unordered_map<Node*, Node*> node_maps;
        new_head->next = new Node(head->val);
        node_maps.insert({head, new_head->next});
        Node* cur = new_head->next;
        while(head) {
            if (head->random) {
                auto iter = node_maps.find(head->random);
                if (iter != node_maps.end()) {
                    cur->random = iter->second;
                } else {
                    cur->random = new Node(head->random->val);
                    node_maps.insert({head->random, cur->random});
                }
            }
            if (head->next)  {
                auto iter = node_maps.find(head->next);
                if (iter != node_maps.end()) {
                    cur->next = iter->second;
                } else {
                    cur->next = new Node(head->next->val);
                    node_maps.insert({head->next, cur->next});
                }
                cur = cur->next;
            }
            head = head->next;
        }
        return new_head->next;
    }
};