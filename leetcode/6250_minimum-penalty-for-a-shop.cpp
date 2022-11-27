#include "common.h"


class Solution {
public:
    int bestClosingTime(string customers) {
        int result = 0;
        int balance = 0;
        for (int i = 0; i < customers.size(); ++i) {
            if (customers[i] == 'N') {
                ++balance;
            } else {
                --balance;
                if (balance < 0) {
                    result = i + 1;
                    balance = 0;
                }
            }
        }
        return result;
    }
};