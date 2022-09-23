#include "common.h"

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        auto iter = nums.begin();
        while(iter != nums.end()) {
            int res = target - *iter;
            if(binary_search(iter + 1, nums.end(), res)) {
                return {res, *iter};
            }
            iter++;
        }
        return {1, 1};
    }

    vector<int> twoSum(vector<int>& nums, int target) {
        int i = 0, j = nums.size() - 1;
        while(i < j) {
            int sum = nums[i] + nums[j];
            if (sum == target) {
                return {nums[i], nums[j]};
            } else if (sum < target) {
                i ++;
            } else {
                j --;
            }
        }
        return {1, 1};
    }
    
};