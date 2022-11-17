#include "common.h"

/*
给你一个非负整数数组 nums ，你最初位于数组的第一个位置。
数组中的每个元素代表你在该位置可以跳跃的最大长度。
你的目标是使用最少的跳跃次数到达数组的最后一个位置。
假设你总是可以到达数组的最后一个位置。

示例 1:

输入: nums = [2,3,1,1,4]
输出: 2
解释: 跳到最后一个位置的最小跳跃数是 2。
     从下标为 0 跳到下标为 1 的位置，跳 1 步，然后跳 3 步到达数组的最后一个位置。
示例 2:

输入: nums = [2,3,0,1,4]
输出: 2
 

提示:

1 <= nums.length <= 104
0 <= nums[i] <= 1000

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/jump-game-ii
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution
{
public:
    /* beat 9.26% */
    int jump_v1(vector<int> &nums)
    {
        vector<int> times(nums.size(), INT_MAX);
        for (int i = 0; i < nums.size() - 1; i++)
        {
            for (int j = i + 1; j <= i + nums[i] && j < nums.size(); j++)
            {
                times[j] = min(times[i] + 1, times[j]);
            }
        }
        return times[nums.size() - 1];
    }

    int jump(vector<int> &nums) {
        int max_pos = 0, end = 0, step = 0;
        for (int i = 0; i < nums.size() - 1; ++i) {
            max_pos = max(max_pos, i + nums[i]);
            if (i == end) {
                end = max_pos;
                ++step;
            }
        }
        return step;
    }
};