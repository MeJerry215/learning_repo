#include <vector>
using std::vector;

#include <algorithm>
using std::all_of;
using std::begin;
using std::binary_search;
using std::end;
using std::fill;
using std::find;
using std::find_first_of;
using std::for_each;
using std::lower_bound;
using std::max;
using std::max_element;
using std::min;
using std::reverse;
using std::sort;
using std::upper_bound;

#include <iostream>
using std::cout;
using std::endl;
using std::sort;

#include <numeric>
using std::abs;
using std::accumulate;
using std::transform;
#include <memory>
using std::move;

#include <utility>
using std::make_pair;
using std::pair;

#include <string>
using std::string;
using std::swap;
using std::to_string;

#include <climits>

#include <queue>
using std::deque;
using std::greater;
using std::less;
using std::priority_queue;
using std::queue;

#include <map>
using std::map;

#include <unordered_map>
using std::unordered_map;

#include <unordered_set>
using std::unordered_set;

#include <cstring>
using std::memset;

#include <cassert>

#include <functional>
using std::function;

#include <sstream>
using std::stringstream;

#include <cmath>
using std::ceil;
using std::floor;
using std::round;
using std::sqrt;

#include <array>
using std::array;

#include <utility>
using std::make_pair;
using std::pair;

using std::tolower;
using std::toupper;

#include <list>
using std::list;

#include <stack>
using std::stack;

#include <numeric>
using std::iota;

#include <bitset>
using std::bitset;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

extern void TestSolution();
int main(int argc, char const *argv[])
{
    TestSolution();
    return 0;
}

template <typename T>
void print_vec(vector<T> &vec)
{
    typedef typename vector<T>::size_type size_type;
    for (size_type i = 0; i < vec.size(); i++)
    {
        cout << vec[i] << "\t";
    }
    cout << endl;
}

template <>
void print_vec(vector<pair<int, int>> &vec)
{
    typedef typename vector<pair<int, int>>::size_type size_type;
    for (size_type i = 0; i < vec.size(); i++)
    {
        cout << vec[i].first << ":" << vec[i].second << "\t";
    }
    cout << endl;
}

template <>
void print_vec(vector<vector<int>> &vec)
{
    typedef typename vector<vector<int>>::size_type size_type;
    for (size_type i = 0; i < vec.size(); i++)
    {
        cout << "(";
        for (int j = 0; j < vec[i].size(); j++)
        {
            cout << vec[i][j] << ",";
        }
        cout << "),";
    }
    cout << endl;
}

template <typename T>
void print_mat(vector<vector<T>> &mat)
{
    typedef typename vector<T>::size_type size_type;
    size_type M = mat.size();
    for (size_type i = 0; i < M; i++)
    {
        for (size_type j = 0; j < mat[i].size(); j++)
        {
            cout << mat[i][j] << "\t";
        }
        cout << endl;
    }
}

// template<const int SIZE>
ListNode *make_list_node(vector<int> arr)
{
    ListNode *head = new ListNode(0);
    ListNode *cur = head;
    for (int i = 0; i < arr.size(); i++)
    {
        cur->next = new ListNode(arr[i]);
        cur = cur->next;
    }
    return head->next;
}

void print_list_node(ListNode *head)
{
    while (head)
    {
        cout << head->val << " ";
        head = head->next;
    }
    cout << endl;
}

TreeNode *make_tree_node(vector<int> arr)
{
    vector<TreeNode *> tmp(arr.size(), nullptr);
    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i] != INT_MIN)
        {
            tmp[i] = new TreeNode(arr[i]);
        }
    }
    // 0 1 2 3 4 5 6 7 8
    /*              0
                /       \
               1         2
            /   \      /   \
           3     4    5     6
    */
    for (int i = 1; i < arr.size(); i++)
    {
        TreeNode *parent = tmp[(i - 1) / 2];
        if (!parent)
            continue;
        if (i % 2 == 0)
        {
            parent->right = tmp[i];
        }
        else
        {
            parent->left = tmp[i];
        }
    }
    return tmp[0];
}

void bfs_tree_node(TreeNode *node)
{
    queue<TreeNode *> node_queue;
    node_queue.push(node);
    while (!node_queue.empty())
    {
        TreeNode *cur = node_queue.front();
        node_queue.pop();
        if (cur)
        {
            cout << cur->val << "\t";
            node_queue.push(cur->left);
            node_queue.push(cur->right);
        }
        else
        {
            cout << "null"
                 << "\t";
        }
    }
    cout << endl;
}
