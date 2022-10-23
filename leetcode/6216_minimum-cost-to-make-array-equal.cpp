#include "common.h"


class Solution {
public:

    /* 带权中位数，把cost[i]看成nums[i] 出现次数 */
    long long minCost(vector<int>& nums, vector<int>& cost) {
        vector<int> idxs(nums.size());
        iota(idxs.begin(),  idxs.end(), 0);
        sort(idxs.begin(), idxs.end(), [&](const int x, const int y) {
            return nums[x] < nums[y];
        });
        long long total = accumulate(cost.begin(), cost.end(), 0L);
        long long sum = 0;
        long long k = 0;
        for (int i = 0; i < idxs.size(); i++) {
            sum += (long long)cost[idxs[i]];
            if (sum >= total / 2) {
                k = nums[idxs[i]];
                break;
            }
        }
        long long res = 0;
        for(int i = 0; i < nums.size(); i++) {
            res += abs((long long)nums[i] - k) * cost[i];
        }
        return res;
    }
};
