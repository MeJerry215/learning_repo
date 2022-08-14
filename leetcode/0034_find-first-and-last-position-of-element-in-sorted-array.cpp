#include "common.h"

/*
给你一个按照非递减顺序排列的整数数组 nums，和一个目标值 target。请你找出给定目标值在数组中的开始位置和结束位置。
如果数组中不存在目标值 target，返回 [-1, -1]。
你必须设计并实现时间复杂度为 O(log n) 的算法解决此问题。

示例 1：
输入：nums = [5,7,7,8,8,10], target = 8
输出：[3,4]
示例 2：
输入：nums = [5,7,7,8,8,10], target = 6
输出：[-1,-1]
示例 3：
输入：nums = [], target = 0
输出：[-1,-1]
提示：
0 <= nums.length <= 105
-109 <= nums[i] <= 109
nums 是一个非递减数组
-109 <= target <= 109
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/find-first-and-last-position-of-element-in-sorted-array
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    vector<int> binary_search_healper(vector<int>& nums, int left, int right, int target) {
        int l_rng = -1, r_rng = -1;
        while(left <= right) {
            int mid = (left + right) / 2;
            if (nums[mid] == target) {
                l_rng = r_rng = mid;
                while (l_rng > 0 && nums[l_rng - 1] == target) {
                    l_rng --;
                }
                while( r_rng < nums.size() - 1 && nums[r_rng + 1] == target) {
                    r_rng ++;
                }
                
                break;
            } else {
                if (nums[mid] < target) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        return {l_rng, r_rng};
    }

    vector<int> searchRange(vector<int>& nums, int target) {
        return binary_search_healper(nums, 0, nums.size() - 1, target);
    }
};

void TestSolution() {
    Solution s;
    vector<int> nums = {5,7,7,8,8, 8,10};
    auto ans = s.searchRange(nums, 8);
    print_vec(ans);
}