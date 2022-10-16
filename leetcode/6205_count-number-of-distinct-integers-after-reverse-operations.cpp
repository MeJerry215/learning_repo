#include "common.h"


/*
给你一个由 正 整数组成的数组 nums 。
你必须取出数组中的每个整数，反转其中每个数位，并将反转后得到的数字添加到数组的末尾。这一操作只针对 nums 中原有的整数执行。
返回结果数组中 不同 整数的数目。

示例 1：
输入：nums = [1,13,10,12,31]
输出：6
解释：反转每个数字后，结果数组是 [1,13,10,12,31,1,31,1,21,13] 。
反转后得到的数字添加到数组的末尾并按斜体加粗表示。注意对于整数 10 ，反转之后会变成 01 ，即 1 。
数组中不同整数的数目为 6（数字 1、10、12、13、21 和 31）。
示例 2：
输入：nums = [2,2,2]
输出：1
解释：反转每个数字后，结果数组是 [2,2,2,2,2,2] 。
数组中不同整数的数目为 1（数字 2）。

提示：
1 <= nums.length <= 105
1 <= nums[i] <= 106

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/count-number-of-distinct-integers-after-reverse-operations
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    int reverse(int v) {
        // 1031 = 1301
        int res = 0;
        while(v) {
            res *= 10;
            res +=(v % 10);
            v /= 10;
        }
        return res;
    }

    int countDistinctIntegers(vector<int>& nums) {
        unordered_set<int> all_nums;
        for(int num: nums) {
            // if (!all_nums.count(num)) { // why
                all_nums.insert(num);
                all_nums.insert(reverse(num));
            // }
        }
        return all_nums.size();
    }
};