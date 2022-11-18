#include "common.h"

/*
给定你一个整数数组 nums
我们要将 nums 数组中的每个元素移动到 A 数组 或者 B 数组中，使得 A 数组和 B 数组不为空，并且 average(A) == average(B) 。
如果可以完成则返回true ， 否则返回 false  。
注意：对于数组 arr ,  average(arr) 是 arr 的所有元素的和除以 arr 长度。

示例 1:
输入: nums = [1,2,3,4,5,6,7,8]
输出: true
解释: 我们可以将数组分割为 [1,4,5,8] 和 [2,3,6,7], 他们的平均值都是4.5。
示例 2:
输入: nums = [3,1]
输出: false

提示:

1 <= nums.length <= 30
0 <= nums[i] <= 104

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/split-array-with-same-average
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

/*
如果暴力枚举，复杂度为O(2 ^ n) 这道题目里面是 2 ^ 30 官解 提供的方法一 折半查找，时间复杂度 2 ^ 15 相对来说小了很大的量集。
*/

class Solution {
public:
    bool splitArraySameAverage(vector<int> &nums) {
        int n = nums.size(), m = n / 2;
        if (n == 1) {
            return false;
        }

        int sum = accumulate(nums.begin(), nums.end(), 0);
        for (int &x : nums) {
            x = x * n - sum;
        }
        cout << endl;
        unordered_set<int> left;
        for (int i = 1; i < (1 << m); i++) {
            int tot = 0;
            for (int j = 0; j < m; j++) {
                if (i & (1 << j)) {
                    tot += nums[j];
                }
            }
            if (tot == 0) {
                return true;
            }
            left.emplace(tot);
        }

        int rsum = accumulate(nums.begin() + m, nums.end(), 0);
        for (int i = 1; i < (1 << (n - m)); i++) {
            int tot = 0;
            for (int j = m; j < n; j++) {
                if (i & (1 << (j - m))) {
                    tot += nums[j];
                }
            }
            if (tot == 0 || (((1 << (n - m)) - 1) != i && left.count(-tot))) {
                return true;
            }
        }
        return false;
    }
};

