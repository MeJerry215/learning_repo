#include "common.h"

/*
整数数组的一个 排列  就是将其所有成员以序列或线性顺序排列。
例如，arr = [1,2,3] ，以下这些都可以视作 arr 的排列：[1,2,3]、[1,3,2]、[3,1,2]、[2,3,1] 。
整数数组的 下一个排列 是指其整数的下一个字典序更大的排列。更正式地，如果数组的所有排列根据其字典顺序从小到大排列在一个容器中，那么数组的 下一个排列 就是在这个有序容器中排在它后面的那个排列。如果不存在下一个更大的排列，那么这个数组必须重排为字典序最小的排列（即，其元素按升序排列）。
例如，arr = [1,2,3] 的下一个排列是 [1,3,2] 。
类似地，arr = [2,3,1] 的下一个排列是 [3,1,2] 。
而 arr = [3,2,1] 的下一个排列是 [1,2,3] ，因为 [3,2,1] 不存在一个字典序更大的排列。
给你一个整数数组 nums ，找出 nums 的下一个排列。
必须 原地 修改，只允许使用额外常数空间。
 

示例 1：
输入：nums = [1,2,3]
输出：[1,3,2]
示例 2：
输入：nums = [3,2,1]
输出：[1,2,3]
示例 3：
输入：nums = [1,1,5]
输出：[1,5,1]
 

提示：
1 <= nums.length <= 100
0 <= nums[i] <= 100

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/next-permutation
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
// 1 4 2 6 7 3 1 -> 1 4 2 7 1 3 6 -> 1 4 2 7 1 6 3 -> 1 4 2 7 6 1 3

class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        int j = nums.size() - 1;
        while(j > 0) {
            if (nums[j] <= nums[j - 1]) j--;
            else {
                int greater_j = j;
                int i = j + 1;
                while(i < nums.size()) {
                    if (nums[i] < nums[greater_j] && nums[i] >  nums[j -1]) {
                        greater_j = i;
                    }
                    i++;
                }
                swap(nums[greater_j], nums[j-1]);
                sort(nums.begin() + j, nums.end());
                break;
            }
        }
        if (j == 0) reverse(nums.begin(), nums.end());
    }
};

void TestSolution() {
    Solution s;
    vector<int> nums = {1, 4, 2, 6, 7, 3, 1};
    s.nextPermutation(nums);
    print_vec(nums);
    s.nextPermutation(nums);
    print_vec(nums);
    s.nextPermutation(nums);
    print_vec(nums);
    s.nextPermutation(nums);
    print_vec(nums);
    nums = {1, 3, 2};
    s.nextPermutation(nums);
    print_vec(nums);
}