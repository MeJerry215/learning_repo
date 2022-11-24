#include "common.h"

/*
给你一个整数数组 nums 和两个整数：left 及 right 。找出 nums 中连续、非空且其中最大元素在范围 [left, right] 内的子数组，并返回满足条件的子数组的个数。
生成的测试用例保证结果符合 32-bit 整数范围。

示例 1：

输入：nums = [2,1,4,3], left = 2, right = 3
输出：3
解释：满足条件的三个子数组：[2], [2, 1], [3]
示例 2：

输入：nums = [2,9,2,5,6], left = 2, right = 8
输出：7
 

提示：

1 <= nums.length <= 105
0 <= nums[i] <= 109
0 <= left <= right <= 109

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/number-of-subarrays-with-bounded-maximum
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    int numSubarrayBoundedMax_v1(vector<int>& nums, int left, int right) {
        int range_max = INT_MIN, res = 0, cnt = 0, out_range_cnt = 0;
        for(int i = 0; i < nums.size(); i++) {
            range_max = max(range_max, nums[i]);
            if (range_max > right) {
                cnt = 0, out_range_cnt = 0, range_max = INT_MIN;
            } else {
                cnt ++;
                if (range_max >= left) {
                    if (nums[i] < left) out_range_cnt ++;
                    else out_range_cnt = 0;
                    res -= out_range_cnt;
                    res += cnt;
                }

            }
        }
        return res;
    }

    /*
    分析: 对于当前的元素存在三种状态 0) nums[i] < left 1) right >= nums[i] >= left 2) nums[i] > right
    将以上三种状态分别记作 last0, last1, last2。则当前区间存在的有效个数 (last2, last1] last1 - last2, last0 对于结果没有贡献
    */
    int numSubarrayBoundedMax_v2(vector<int>& nums, int left, int right) {
        int res = 0, last1 = -1, last2 = -1;
        for(int i = 0; i < nums.size(); i++) {
            if (nums[i] > right) {
                last2 = i;
            } else if (nums[i] >= left) {
                last1 = i;
            }
            if (last1 > last2)  {
                res += (last1 - last2);
            }
        }
        return res;
    }

    /*
    要计算的合法子区间不包含 2 且至少包含一个 1。所以，我们可以先求出只包含 0 或 1 的子区间数目，再减去只包括 0 的子区间数目
    */
    int numSubarrayBoundedMax(vector<int>& nums, int left, int right) {
        return count(nums, right) - count(nums, left - 1);
    }

    int count(vector<int>& nums, int lower) {
        int res = 0, cur = 0;
        for (auto x : nums) {
            cur = x <= lower ? cur + 1 : 0;
            res += cur;
        }
        return res;
    }


};

void TestSolution() {
    Solution s;
    vector<int> nums = {876,880,482,260,132,421,732,703,795,420,871,445,400,291,358,589,617,202,755,810,227,813,549,791,418,528,835,401,526,584,873,662,13,314,988,101,299,816,833,224,160,852,179,769,646,558,661,808,651,982,878,918,406,551,467,87,139,387,16,531,307,389,939,551,613,36,528,460,404,314,66,111,458,531,944,461,951,419,82,896,467,353,704,905,705,760,61,422,395,298,127,516,153,299,801,341,668,598,98,241};
    cout << s.numSubarrayBoundedMax_v2(nums, 658, 719) << endl;
}
// 1   1   1   1   1   1   1   1
// 0   1   2   3    4  5   6   7   8   9   10  11  12  13  14
// 876,880,482,260,132,421,732,703,795,420,871,445,400,291,358,589,617,202,755,810,227,813,549,791,418,528,835,401,526,584,873,662,13,314,988,101,299,816,833,224,160,852,179,769,646,558,661,808,651,982,878,918,406,551,467,87,139,387,16,531,307,389,939,551,613,36,528,460,404,314,66,111,458,531,944,461,951,419,82,896,467,353,704,905,705,760,61,422,395,298,127,516,153,299,801,341,668,598,98,241]
// 658
// 719
