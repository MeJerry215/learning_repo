#include "common.h"

/*
给你一个数组 nums 。nums 的源数组中，所有元素与 nums 相同，但按非递减顺序排列。
如果 nums 能够由源数组轮转若干位置（包括 0 个位置）得到，则返回 true ；否则，返回 false 。
源数组中可能存在 重复项 。
注意：我们称数组 A 在轮转 x 个位置后得到长度相同的数组 B ，当它们满足 A[i] == B[(i+x) % A.length] ，其中 % 为取余运算。

示例 1：
输入：nums = [3,4,5,1,2]
输出：true
解释：[1,2,3,4,5] 为有序的源数组。
可以轮转 x = 3 个位置，使新数组从值为 3 的元素开始：[3,4,5,1,2] 。
示例 2：
输入：nums = [2,1,3,4]
输出：false
解释：源数组无法经轮转得到 nums 。
示例 3：
输入：nums = [1,2,3]
输出：true
解释：[1,2,3] 为有序的源数组。
可以轮转 x = 0 个位置（即不轮转）得到 nums 。

提示：
1 <= nums.length <= 100
1 <= nums[i] <= 100

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/check-if-array-is-sorted-and-rotated
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    bool check(vector<int>& nums) {
        bool reset_flag = false;
        for(int i = 1; i < nums.size(); i++) {
            if (nums[i] >= nums[i - 1]) continue;
            else if (reset_flag) return false;
            else {
                reset_flag = true;
                if (nums[nums.size() - 1] > nums[0]) return false;
            }
        }
        return true;
    }
};