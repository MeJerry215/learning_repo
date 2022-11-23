#include "common.h"


class Solution {
public:
    int longestOnes(vector<int>& nums, int k) {
        int res = 0;
        int l = 0, r = 0, flip_cnt = 0;
        while(r < nums.size()) {
            if (nums[r] == 0) {
                flip_cnt ++;
            }
            while(flip_cnt > k && l <= r) {
                if (nums[l] == 0) {
                    flip_cnt --;
                }
                l++;
            }
            res = max(res, r - l + 1);
            r++;
        }
        return res;
    }
};