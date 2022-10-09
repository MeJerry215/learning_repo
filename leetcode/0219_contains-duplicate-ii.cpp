#include "common.h"


/*
可以使用滑动窗口来优化，如果滑动窗口内存在重复元素，有效
*/
class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        unordered_map<int, int> last_postion;
        for(int i = 0; i < nums.size(); i++) {
            if (last_postion.count(nums[i]) > 0) {
                if (i - last_postion[nums[i]] <= k) {
                    return true;
                }
            }
            last_postion[nums[i]] = i;
        }
        return false;
    }
};