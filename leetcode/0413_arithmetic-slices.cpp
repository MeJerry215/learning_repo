#include "common.h"

/*
如果一个数列 至少有三个元素 ，并且任意两个相邻元素之差相同，则称该数列为等差数列。
例如，[1,3,5,7,9]、[7,7,7,7] 和 [3,-1,-5,-9] 都是等差数列。
给你一个整数数组 nums ，返回数组 nums 中所有为等差数组的 子数组 个数。
子数组 是数组中的一个连续序列。

示例 1：
输入：nums = [1,2,3,4]
输出：3
解释：nums 中有三个子等差数组：[1, 2, 3]、[2, 3, 4] 和 [1,2,3,4] 自身。
示例 2：
输入：nums = [1]
输出：0
提示：

1 <= nums.length <= 5000
-1000 <= nums[i] <= 1000

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/arithmetic-slices
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        /* for a arithmetic array, only need two num to know the difference */
        // 1 2 3
        //  推导一下 对于 3-5 长度的seq 有多少个数列呢 
        // 1 2 3     1 2 3 4        1 2 3 4 5
        //           1 2 3          1 2 3 4
        //              2 3 4         2 3 4 5  
        //                          1 2 3
        //                            2 3 4  
        //                             3 4 5
        //  1        1 + 2           1 + 2 + 3
        // 接上面说 在n = 5的时候，多出来三项，而多出来的这三项是哪三项，定然是原来长度为2的时候多的数组，所以你用长度3的滑动窗在数组上滑动，可以得到n - 2个窗
        int res = 0;
        if (nums.size() < 3) return 0; 
        int d = nums[1] - nums[0];
        int start = 0;
        for(int i = 2; i < nums.size(); i++) {
            if (nums[i] - nums[i - 1] == d) {
                res += (i - start - 1);
            } else {
                d = nums[i] - nums[i - 1];
                start = i - 1;
            }
        }
        return res;
           
    }
};