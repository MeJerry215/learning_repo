#include "common.h"

/*
给你一个下标从 0 开始的数组 nums ，它含有 n 个非负整数。
每一步操作中，你需要：
选择一个满足 1 <= i < n 的整数 i ，且 nums[i] > 0 。
将 nums[i] 减 1 。
将 nums[i - 1] 加 1 。
你可以对数组执行 任意 次上述操作，请你返回可以得到的 nums 数组中 最大值 最小 为多少。
 

示例 1：
输入：nums = [3,7,1,6]
输出：5
解释：
一串最优操作是：
1. 选择 i = 1 ，nums 变为 [4,6,1,6] 。
2. 选择 i = 3 ，nums 变为 [4,6,2,5] 。
3. 选择 i = 1 ，nums 变为 [5,5,2,5] 。
nums 中最大值为 5 。无法得到比 5 更小的最大值。
所以我们返回 5 。
示例 2：
输入：nums = [10,1]
输出：10
解释：
最优解是不改动 nums ，10 是最大值，所以返回 10 。

提示：

n == nums.length
2 <= n <= 105

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/minimize-maximum-of-array
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    static bool check(vector<int> &nums, int k) {
        long long have = 0;//前方的数字还可以帮我们后方的大数承载多少数字
        for (int n : nums) {
            if (n <= k) {
                have += k - n;//较小数，可以算入承载量
            } else {
                if (have < n - k) return 0;//承载不了了，该答案不可行
                else have -= (n - k);//减去承载量
            }
        }
        return 1;
    }

    int minimizeArrayValue_v1(vector<int> &nums) {
        int left = 0, right = *max_element(nums.begin(), nums.end());
        while (left < right) {//二分答案，寻找最大值
            int mid = left + (right - left) / 2;
            if (check(nums, mid)) right = mid;
            else left = mid + 1;
        }
        return left;
    }

    int minimizeArrayValue(vector<int>& nums)
    {
        int res = 0;
        double sum = 0.0;
        for(int i = 0; i < nums.size(); i ++) {
            sum += nums[i];
            int upper = ceil(sum / (i + 1));
            res = max(res, upper);
        }
        return res;
    }

};

/*
考虑数组 可以对nums[i] - 1, 产生nums[i - 1] + 1的效果
    1  33   2   5  66   4   5
这道题目大概率是题dp问题，所以需要算的就是找规律

    1              此时最大最小结果就是 1
    1  33          此时最大值为33, 所以要33匀一点给1，为了使得最大值最小呢？两个数相近，为 (1 + 33) /2， 数组变为 17 17
    17 17  2       2当前什么都不用做，做了也没啥只会让前面的数增大，此时 为 17
    17 17  2  5    依旧什么都不用。
    17 17  2  5  66  此时可以什么都不做最大值为66，尽可能的往前移，ceil(107 / 5) 均值在22
    22 22  22 22 19 从以上变化可以看到只要当前算出来的ceil值大于之前的最大值，总可以转换为如上规则形式

*/