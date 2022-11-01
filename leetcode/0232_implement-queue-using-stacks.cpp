#include "common.h"

class MyQueue
{
    stack<int> push_st, pop_st;

public:
    MyQueue() {}

    void push(int x)
    {
        push_st.push(x);
    }

    int pop()
    {
        int val = peek();
        if (!pop_st.empty())
            pop_st.pop();
        return val;
    }

    int peek()
    {
        if (!pop_st.empty())
            return pop_st.top();
        if (push_st.empty())
            return -1;
        while (!push_st.empty())
        {
            pop_st.push(push_st.top());
            push_st.pop();
        };
        return pop_st.top();
    }

    bool empty()
    {
        return push_st.empty() && pop_st.empty();
    }
};