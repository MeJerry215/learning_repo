#include "common.h"

class Solution
{
public:
    void rotate(vector<int> &nums, int k) {
        // rotate k times means no rotate
        k = k % nums.size();
        // this is the basic version of ratate, it shift only one element one time. so the time cost will not so good.
        /*
        for(int i = 0; i < k; i++) {
            int tmp = nums[nums.size() - 1];
            for(int j = nums.size() - 1; j > 0; j--) {
              nums[j] = nums[j - 1];
            }
            nums[0] = tmp;
        }
        */
    }
};