#include "common.h"


class Solution {
public:
    int largestAltitude(vector<int>& gain) {
        int res = 0, sum = 0;
        for(auto num: gain) {
            sum += num;
            res = max(res, sum);
        }
        return res;
    }
};