#include "common.h"

/*
给定一个长度为偶数的整数数组 arr，只有对 arr 进行重组后可以满足 “对于每个 0 <= i < len(arr) / 2，都有 arr[2 * i + 1] = 2 * arr[2 * i]” 时，返回 true；否则，返回 false。
示例 1：
输入：arr = [3,1,3,6]
输出：false
示例 2：
输入：arr = [2,1,2,6]
输出：false
示例 3：
输入：arr = [4,-2,2,-4]
输出：true
解释：可以用 [-2,-4] 和 [2,4] 这两组组成 [-2,-4,2,4] 或是 [2,4,-2,-4]

提示：
0 <= arr.length <= 3 * 104
arr.length 是偶数
-105 <= arr[i] <= 105
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/array-of-doubled-pairs
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    bool canReorderDoubled(vector<int>& arr) {
        unordered_map<int, int> num_cnt;
        for(auto num: arr) {
            num_cnt[num] ++;
        }
        vector<int> keys;
        for(auto entry: num_cnt) {
            keys.push_back(entry.first);
        }
        sort(keys.begin(), keys.end(), [](const int lhs, const int rhs) {
            return abs(lhs) < abs(rhs);
        });
        for(auto key: keys) {
            num_cnt[key * 2] -= num_cnt[key];
            if (num_cnt[key * 2] < 0) return false;
        }
        return true;
    }
};