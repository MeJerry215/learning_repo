#include "common.h"

/*
You are given an array arr which consists of only zeros and ones, divide the array into three non-empty parts such that all of these parts represent the same binary value.

If it is possible, return any [i, j] with i + 1 < j, such that:

arr[0], arr[1], ..., arr[i] is the first part,
arr[i + 1], arr[i + 2], ..., arr[j - 1] is the second part, and
arr[j], arr[j + 1], ..., arr[arr.length - 1] is the third part.
All three parts have equal binary values.
If it is not possible, return [-1, -1].
Note that the entire part is used when considering what binary value it represents. For example, [1,1,0] represents 6 in decimal, not 3. Also, leading zeros are allowed, so [0,1,1] and [1,1] represent the same value.
Example 1:
Input: arr = [1,0,1,0,1]
Output: [0,3]
Example 2:
Input: arr = [1,1,0,1,1]
Output: [-1,-1]
Example 3:

Input: arr = [1,1,0,0,1]
Output: [0,2]
 

Constraints:

3 <= arr.length <= 3 * 104
arr[i] is 0 or 1

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/three-equal-parts
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    vector<int> threeEqualParts(vector<int>& arr) {
        int sum = accumulate(arr.begin(), arr.end(), 0);
        if (sum % 3 != 0) {
            return {-1, -1};
        }
        if (sum == 0) {
            return {0,  (int)arr.size() - 1};
        }

        int partial = sum / 3;
        int first = 0, second = 0, third = 0, cur = 0;
        for (int i = 0; i < arr.size(); i++) {
            if (arr[i] == 1) {
                if (cur == 0) {
                    first = i;
                } else if (cur == partial) {
                    second = i;
                } else if (cur == 2 * partial) {
                    third = i;
                }
                cur++;
            }
        }

        int len = (int)arr.size() - third;
        if (first + len <= second && second + len <= third) {
            int i = 0;
            while (third + i < arr.size()) {
                if (arr[first + i] != arr[second + i] || arr[first + i] != arr[third + i]) {
                    return {-1, -1};
                }
                i++;
            }
            return {first + len - 1, second + len};
        }
        return {-1, -1};
    }
};


void TestSolution() {
    Solution s;
    /*
        0   1   2   3   4   5   6   7   8   9
        0   1 | 0   1   1   0   1 | 1   0   1
    */
    vector<int> arr = {0,1,0,1,1,0,1,1,0,1};
    auto res = s.threeEqualParts(arr);
    print_vec(res);
}