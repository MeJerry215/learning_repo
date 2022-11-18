#include "common.h"
/*
给定一个非负整数数组 nums 和一个整数 m ，你需要将这个数组分成 m 个非空的连续子数组。
设计一个算法使得这 m 个子数组各自和的最大值最小。
 

示例 1：

输入：nums = [7,2,5,10,8], m = 2
输出：18
解释：
一共有四种方法将 nums 分割为 2 个子数组。 
其中最好的方式是将其分为 [7,2,5] 和 [10,8] 。
因为此时这两个子数组各自的和的最大值为18，在所有情况中最小。
示例 2：

输入：nums = [1,2,3,4,5], m = 2
输出：9
示例 3：

输入：nums = [1,4,4], m = 3
输出：4
 

提示：

1 <= nums.length <= 1000
0 <= nums[i] <= 106
1 <= m <= min(50, nums.length)

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/split-array-largest-sum
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    int splitArray(vector<int>& nums, int k) {
        int low = *max_element(nums.begin(), nums.end()), high = accumulate(nums.begin(), nums.end(), 0);
        function<int(int)> check = [&](int max_val) {
            int sum = 0;
            int parts = 1;
            for(auto num : nums) {
                sum += num;
                if (sum > max_val) {
                    sum = num;
                    parts ++;
                }
            }
            return parts;
        };
        while(low < high) {
            int mid = low + (high - low) / 2;
            int parts = check(mid);
            if (parts == k) {
                high = mid;
            }
        }
    }
};