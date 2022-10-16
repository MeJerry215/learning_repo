#include "common.h"


class Solution {
public:

    int findMaxK(vector<int>& nums) {
        int res = -1;
        bool counts[2001] = {false};
        for(int num: nums) {
            counts[num + 1000] = true;
            if (counts[1000 - num] && counts[num + 1000]) res = max(res, abs(num));
        }
        return res;
    }
};



