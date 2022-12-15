#include "common.h"

class Solution {
public:
    long long minimalKSum(vector<int>& nums, int k) {
        nums.push_back(0);
        nums.push_back(int(2e9) + 1);
        sort(nums.begin(), nums.end());

        long long presum = 0, ans = 0;
        for (int i = 1; i < nums.size(); ++i) {
            int offer = nums[i] - nums[i - 1] - 1;
            if (offer > 0) {
                if (offer < k) {
                    k -= offer;
                } else {
                    ans = static_cast<long long>(nums[i - 1] + k + 1) * (nums[i - 1] + k) / 2 - presum;
                    break;
                }
            }
            if (nums[i] != nums[i - 1]) {
                presum += nums[i];
            }
        }

        return ans;
    }
};



