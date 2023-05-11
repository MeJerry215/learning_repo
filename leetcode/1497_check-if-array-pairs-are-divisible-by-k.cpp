#include "common.h"

/*
Given an array of integers arr of even length n and an integer k.
We want to divide the array into exactly n / 2 pairs such that the sum of each pair is divisible by k.
Return true If you can find a way to do that or false otherwise.

Example 1:
Input: arr = [1,2,3,4,5,10,6,7,8,9], k = 5
Output: true
Explanation: Pairs are (1,9),(2,8),(3,7),(4,6) and (5,10).
Example 2:
Input: arr = [1,2,3,4,5,6], k = 7
Output: true
Explanation: Pairs are (1,6),(2,5) and(3,4).
Example 3:
Input: arr = [1,2,3,4,5,6], k = 10
Output: false
Explanation: You can try all possible pairs to see that there is no way to divide arr into 3 pairs each with sum divisible by 10.

Constraints:
arr.length == n
1 <= n <= 105
n is even.
-109 <= arr[i] <= 109
1 <= k <= 105
*/


/*
这个问题和数值大小无关,只和arr 对于k 取余数后的数字个数有关,由于和不固定,所以可以看余数的和是否固定.
可以将arr 划分到k个桶里面,对应的桶 就是对k取得余数值, 要满足可以两两配对,所以就需要实现 i的桶和 k - i的桶相等.
同时存在特殊场景k = 6的时候 存在 桶3 里面的要成对出现，以及k == 0
*/

class Solution
{
public:
    bool canArrange(vector<int>& arr, int k)
    {
        vector<int> bins(k, 0);

        for(auto num : arr) {
            bins[(num % k + k) % k]++;
        }

        for(int i = 1; i <= k / 2; i++) {
            int a = bins[i], b = bins[k - i];

            if (a != b) { return false; }
        }

        return (bins[0] + 1) % 2;
    }
};


void TestSolution()
{
    Solution s;
    vector<int> arr = {-1, -1, -1, -1, 2, 2, -2, -2};
    cout << s.canArrange(arr, 3) << endl;;
}