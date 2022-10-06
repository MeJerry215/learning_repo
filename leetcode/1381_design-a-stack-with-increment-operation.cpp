#include "common.h"

class CustomStack {
    vector<int> _stack;
    int tail;
    int capacity;
public:
    CustomStack(int maxSize): _stack(vector<int>(maxSize)), tail(-1), capacity(maxSize) {}

    void push(int x) {
        // full stack do nothing
        // if ((tail + 1) % capacity == head) return;
        if (tail + 1 == capacity) return;
        tail = tail + 1;
        _stack[tail] = x;
    }
    
    int pop() {
        // emtpy stack
        if (tail == -1) return -1;
        int res = _stack[tail];
        tail--;
        return res;
    }
    
    void increment(int k, int val) {
        int n_elem = min(k - 1, tail) + 1;
        for(int i = 0; i < n_elem; i++) {
            _stack[i] += val;
        }
    }

};

/**
 * Your CustomStack object will be instantiated and called as such:
 * CustomStack* obj = new CustomStack(maxSize);
 * obj->push(x);
 * int param_2 = obj->pop();
 * obj->increment(k,val);
 */

void TestSolution() {
    CustomStack customStack(3);
    customStack.push(1);
    customStack.push(2);
    customStack.debug();
    customStack.pop();
    customStack.debug();
    customStack.push(2);
    customStack.push(3);
    customStack.debug();
    customStack.push(4);
    customStack.increment(5, 100);
    customStack.debug();
    customStack.increment(2, 100);
    customStack.debug();
    cout << customStack.pop() << endl;
    cout << customStack.pop() << endl;
    cout << customStack.pop() << endl;
    cout << customStack.pop() << endl;
}