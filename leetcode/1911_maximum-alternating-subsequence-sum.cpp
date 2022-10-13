#include "common.h"


class Solution {
public:
    /*
    nums[i]总是大于0，可以少考虑一种情况
    如果nums[i] > 0, 则最大交错数组的长度必然是一个奇数, 如果nums[i]的小于0的场景下，则需要考虑一下结果偶数的可能性质
    对于 任意的序列  nums[i-1], nums[i], nums[i+1], 由于nums[i+1]总是大于0的，只有在nums[i-1] - nums[i] > 0 的时候, nums[i+1]才会加上前面的数
    所以将每个元素当前项，减去后一项，得到一个新的序列，而这个序列表示前后项之间的关系, 对于尾项，由于没有项可以减去，所以增加一个哨兵节点等价于最后的值
    所以根据以上结论, 存在奇数遍历和偶数遍历两种场景，遍历序列每次+2
    arr        4, 2, 5, 3, 1, 8, 2, 6, 5, 5
               2     2    -7    -4     0
                 -3     2     6     1
    arr        4, 2, 5, 3, 3, 1, 4, 2, 5, 5
               2     2     2     2     0
                 -3     0     -3    -3
    arr       99,96, 98, 97, 1, 8, 2, 6, 5, 5
               3     1      -7    -4     0
                 -2      96     6     1
    */
    long long maxAlternatingSum(vector<int>& nums) {
        vector<int> diff(nums.size(), 0);
        for(int i = 0; i < nums.size() - 1; i++) {
            diff[i] = nums[i] - nums[i + 1];
        }
        long long res = 0;
        long long sum = nums[0];
        for(int i = 0; i < nums.size(); i++) {
            res = max(res, (long long)nums[i]);
        }
        for(int i = 2; i < nums.size(); i+=2) {
            if (diff[i - 2] <= 0) {
                sum = nums[i];
            } else {
                sum += nums[i];
                sum -= nums[i - 1];
                res = max(res, sum);
            }
        }
        sum = nums[1];
        for(int i = 3; i < nums.size(); i+=2) {
            if (diff[i - 2] <= 0) {
                sum = nums[i];
            } else {
                sum += nums[i];
                sum -= nums[i - 1];
                res = max(res, sum);
            }
        }
        return res;
    }
};