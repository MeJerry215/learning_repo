#include "common.h"

/*
给定一个数组，包含从 1 到 N 所有的整数，但其中缺了两个数字。你能在 O(N) 时间内只用 O(1) 的空间找到它们吗？
以任意顺序返回这两个数字均可。
示例 1:
输入: [1]
输出: [2,3]
示例 2:
输入: [2,3]
输出: [1,4]
提示：
nums.length <= 30000
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/missing-two-lcci
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution {
public:
    vector<int> missingTwo_v1(vector<int>& nums) {
        nums.resize(nums.size() + 2);
        vector<int> ans;
        int i = 0;
        while(i < nums.size()) {
            if (nums[i] != 0 && nums[i] != i + 1) {
                swap(nums[i], nums[nums[i] - 1]);
                continue;
            }
            i++;
        }
        for(int i = 0; i < nums.size(); i++) {
            if (nums[i] != i + 1) {
                ans.push_back(i + 1);
            }
        }
        return ans;
    }

    vector<int> missingTwo(vector<int>& nums) {
        long n = nums.size() + 2;
        int a = -accumulate(nums.cbegin(), nums.cend(), -(1 + n) * n / 2);
        int b = -inner_product(nums.cbegin(), nums.cend(), nums.cbegin(), -(1 + n) * n / 2 * (2 * n + 1) / 3);
        int tmp = sqrt(2 * b - a * a);
        return { (a + tmp) / 2, (a - tmp) / 2 };
    }

};


void TestSolution() {
    Solution s;
    vector<int> nums = {8, 9, 10, 7, 5, 1, 3, 2, 4};
    auto res = s.missingTwo(nums);
    print_vec(res);
}