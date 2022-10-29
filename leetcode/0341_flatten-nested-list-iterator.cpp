#include "common.h"



class NestedInteger {
public:
    // Return true if this NestedInteger holds a single integer, rather than a nested list.
    bool isInteger() const;

    // Return the single integer that this NestedInteger holds, if it holds a single integer
    // The result is undefined if this NestedInteger holds a nested list
    int getInteger() const;

    // Return the nested list that this NestedInteger holds, if it holds a nested list
    // The result is undefined if this NestedInteger holds a single integer
    const vector<NestedInteger> &getList() const;
};


class NestedIterator {
public:
    vector<int> nums;
    int cur;

    NestedIterator(vector<NestedInteger> &nestedList): cur(0) {
        function<void(const NestedInteger&)> dfs = [&](const NestedInteger& integer) -> void {
            if (integer.isInteger()) {
                nums.push_back(integer.getInteger());
            } else {
                const vector<NestedInteger>& list = integer.getList();
                for (int i = 0; i < list.size(); i++) {
                    dfs(list[i]);
                }
            }
        };
        for(int i = 0; i < nestedList.size(); i++) {
            dfs(nestedList[i]);
        }
    }

    int next() {
        return nums[cur++];
    }

    bool hasNext() {
        return cur < nums.size();
    }
};


class NestedIterator {
private:
    // pair 中存储的是列表的当前遍历位置，以及一个尾后迭代器用于判断是否遍历到了列表末尾
    stack<pair<vector<NestedInteger>::iterator, vector<NestedInteger>::iterator>> stk;

public:
    NestedIterator(vector<NestedInteger> &nestedList) {
        stk.emplace(nestedList.begin(), nestedList.end());
    }

    int next() {
        // 由于保证调用 next 之前会调用 hasNext，直接返回栈顶列表的当前元素，然后迭代器指向下一个元素
        return stk.top().first++->getInteger();
    }

    bool hasNext() {
        while (!stk.empty()) {
            auto &p = stk.top();
            if (p.first == p.second) { // 遍历到当前列表末尾，出栈
                stk.pop();
                continue;
            }
            if (p.first->isInteger()) {
                return true;
            }
            // 若当前元素为列表，则将其入栈，且迭代器指向下一个元素
            auto &lst = p.first++->getList();
            stk.emplace(lst.begin(), lst.end());
        }
        return false;
    }
};
