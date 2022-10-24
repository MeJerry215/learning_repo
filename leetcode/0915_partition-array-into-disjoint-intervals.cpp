#include "common.h"

/*
给定一个数组 nums ，将其划分为两个连续子数组 left 和 right， 使得：

left 中的每个元素都小于或等于 right 中的每个元素。
left 和 right 都是非空的。
left 的长度要尽可能小。
在完成这样的分组后返回 left 的 长度 。

用例可以保证存在这样的划分方法。

 

示例 1：

输入：nums = [5,0,3,8,6]
输出：3
解释：left = [5,0,3]，right = [8,6]
示例 2：

输入：nums = [1,1,1,0,6,12]
输出：4
解释：left = [1,1,1,0]，right = [6,12]
 

提示：

2 <= nums.length <= 105
0 <= nums[i] <= 106
可以保证至少有一种方法能够按题目所描述的那样对 nums 进行划分。

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/partition-array-into-disjoint-intervals
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/


class Solution {
public:
    int partitionDisjoint(vector<int>& nums) {
        vector<int> min_nums(nums.size());
        int min_num = INT_MAX;
        for(int i = nums.size() - 1; i >= 0; i--) {
            min_num = min(nums[i], min_num);
            min_nums[i] = min_num;
        }
        int max_num = INT_MIN;
        print_vec(min_nums);
        for(int i = 1; i < nums.size(); i++) {
            max_num = max(max_num, nums[i - 1]);
            if (max_num <= min_nums[i]) {
                return i;
            }
        }
        return 0;
    }
};


void TestSolution() {
    Solution s;
    vector<int> nums = {1,1,1,0,6,12};
    cout << s.partitionDisjoint(nums) << endl;
}