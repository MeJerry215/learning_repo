#include "common.h"

/*
序列是一个 方波 ：
子序列的长度至少为 2 ，并且
将子序列从小到大排序 之后 ，除第一个元素外，每个元素都是前一个元素的 平方 。
返回 nums 中 最长方波 的长度，如果不存在 方波 则返回 -1 。

子序列 也是一个数组，可以由另一个数组删除一些或不删除元素且不改变剩余元素的顺序得到。

 

示例 1 ：

输入：nums = [4,3,6,16,8,2]
输出：3
解释：选出子序列 [4,16,2] 。排序后，得到 [2,4,16] 。
- 4 = 2 * 2.
- 16 = 4 * 4.
因此，[4,16,2] 是一个方波.
可以证明长度为 4 的子序列都不是方波。
示例 2 ：

输入：nums = [2,3,5,6,7]
输出：-1
解释：nums 不存在方波，所以返回 -1 。
 

提示：

2 <= nums.length <= 105
2 <= nums[i] <= 105

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/longest-square-streak-in-an-array
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    int longestSquareStreak(vector<int>& nums) {
        if (nums.size() < 2) return -1;
        sort(nums.begin(), nums.end());
        int res = 0;
        unordered_map<int, int> cnts;
        for(auto num: nums) {
            int root = sqrt(num);
            if (root * root != num || !cnts.count(root)) cnts[num] = 1;
            else cnts[num] = cnts[root] + 1;
            res = max(res, cnts[num]);
        }
        return res > 1 ? res : -1;
    }
};