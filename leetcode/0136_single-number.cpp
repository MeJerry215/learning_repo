#include "common.h"

class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int xor_val = 0;
        for(auto num: nums) xor_val ^= num;
        return xor_val;
    }
};