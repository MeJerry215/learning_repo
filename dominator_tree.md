# Dominator Tree

在有向图中，如果u到v之间，必须经过w节点，那么就称u被w支配。找到这个w的过程也就是支配树算法，而在支配树算法中要使用的一个算法是LCA(Least Common Ancestor)。

https://www.cnblogs.com/fenghaoran/p/dominator_tree.html

```c++
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <deque>
struct Edge;
struct Node
{
    Node(int x) : key(x){};
    int key;
    std::vector<Edge *> children;
};

struct Edge
{
    Edge(Node *n) : node(n) {}
    Node *node{nullptr};
};

Node *get_node(std::unordered_map<int, Node *> &nodes, int key)
{
    auto iter = nodes.find(key);
    if (iter != nodes.end())
    {
        return iter->second;
    }
    else
    {
        Node *node = new Node(key);
        nodes.insert({key, node});
        return node;
    }
}

Node *construct(int head, std::vector<std::vector<int>> &edges)
{
    std::unordered_map<int, Node *> nodes;
    Node *headNode = new Node(head);
    nodes.insert({head, headNode});
    for (auto &edge : edges)
    {
        int src_key = edge[0];
        int dst_key = edge[1];
        auto src_iter = nodes.find(src_key);
        Node *src_node = get_node(nodes, src_key);
        Node *dst_node = get_node(nodes, dst_key);
        Edge *link = new Edge(dst_node);
        src_node->children.push_back(link);
    }
    return headNode;
}

void display_graph(Node *root)
{
    std::vector<Node *> visited;
    std::deque<Node *> toVisit;
    toVisit.push_back(root);
    while (toVisit.size() > 0)
    {
        Node *tmp = toVisit.front();
        toVisit.pop_front();
        if (std::find(visited.begin(), visited.end(), tmp) != visited.end())
            continue;
        visited.push_back(tmp);
        std::cout << "node " << tmp->key << " has child [";
        for (int i = 0; i < tmp->children.size(); i++)
        {
            Edge *child = tmp->children[i];
            std::cout << child->node->key << ",";
            toVisit.push_back(child->node);
        }
        std::cout << "]" << std::endl;
    }
}

void topolog_sort(vector<Node*> nodes) {
    
}

int main(int argc, char const *argv[])
{
    /*
                    0
                  /   \
                 1    | \
               /  \  /   6
              2    3    / \
               \  /    7   8
                 4 ---/    |
                 |        /
                 5 ------/
    */
    std::vector<std::vector<int>> edges = {
        {1, 2}, {1, 3}, {2, 4}, {3, 4}, {4, 5}, {0, 1}, {0, 3}, {0, 6}, {6, 7}, {7, 4}, {6, 8}, {8, 5}};
    Node *root = construct(0, edges);
    display_graph(root);
    return 0;
}

```

