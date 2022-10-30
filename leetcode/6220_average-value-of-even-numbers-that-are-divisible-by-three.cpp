#include "common.h"


class Solution {
public:
    int averageValue(vector<int>& nums) {
        int cnt = 0;
        int sum = 0;
        for(auto num: nums) {
            if (num % 6 == 0) {
                cnt ++;
                sum += num;
            }
        }
        return cnt ? sum / cnt : cnt;
    }
};