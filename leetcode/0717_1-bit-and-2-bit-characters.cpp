#include "common.h"


class Solution {
public:
    bool isOneBitCharacter(vector<int>& bits) {
        if (bits.size() == 1) return true;
        int n = bits.size();
        if (bits[n - 1] == bits[n - 2]) return true;
        if (n >= 3 && bits[n - 2] == bits[n - 3] && bits[n - 2] == 1) {
            int one_cnt = 0;
            int i = n - 2;
            while(i >= 0 && bits[i--]) {
                one_cnt++;
            }
            return !(one_cnt % 2);
        }
        return false;
    }
};