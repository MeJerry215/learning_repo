#include "common.h"


class StockSpanner {
public:
    StockSpanner() {
        idx = -1;
        dec_stk.push({INT_MAX, -1});
    }

    int next(int price) {
        idx ++;
        while( dec_stk.top().first <= price) {
            dec_stk.pop();
        }
        int res = idx - dec_stk.top().second;
        dec_stk.push({price, idx});
        return res;
    }

private:
    int idx;
    stack<pair<int, int>> dec_stk;
};


/**
 * Your StockSpanner object will be instantiated and called as such:
 * StockSpanner* obj = new StockSpanner();
 * int param_1 = obj->next(price);
 */