#include "common.h"

class Solution
{
public:
    vector<int> targetIndices(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        auto high = upper_bound(nums.begin(), nums.end(), target);
        auto low = lower_bound(nums.begin(), nums.end(), target);
        vector<int> res(high - low);
        iota(res.begin(), res.end(), low - nums.begin());
        return res;
    }
};