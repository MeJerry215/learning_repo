#include "common.h"

/*
给定一个整数数组 arr，找到 min(b) 的总和，其中 b 的范围为 arr 的每个（连续）子数组。
由于答案可能很大，因此 返回答案模 10^9 + 7 。

示例 1：
输入：arr = [3,1,2,4]
输出：17
解释：
子数组为 [3]，[1]，[2]，[4]，[3,1]，[1,2]，[2,4]，[3,1,2]，[1,2,4]，[3,1,2,4]。
最小值为 3，1，2，4，1，1，2，1，1，1，和为 17。
示例 2：
输入：arr = [11,81,94,43,3]
输出：444

提示：
1 <= arr.length <= 3 * 104
1 <= arr[i] <= 3 * 104

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/sum-of-subarray-minimums
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
    const int MOD = 1e9 + 7;
public:
    /* 三次遍历 */
    int sumSubarrayMins_v1(vector<int> &arr) {
        int n = arr.size();
        // 左边界 left[i] 为左侧严格小于 arr[i] 的最近元素位置（不存在时为 -1）
        vector<int> left(n, -1);
        stack<int> st;
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && arr[st.top()] >= arr[i])
                st.pop(); // 移除无用数据
            if (!st.empty()) left[i] = st.top();
            st.push(i);
        }

        // 右边界 right[i] 为右侧小于等于 arr[i] 的最近元素位置（不存在时为 n）
        vector<int> right(n, n);
        while (!st.empty()) st.pop();
        for (int i = n - 1; i >= 0; --i) {
            while (!st.empty() && arr[st.top()] > arr[i])
                st.pop(); // 移除无用数据
            if (!st.empty()) right[i] = st.top();
            st.push(i);
        }

        long ans = 0L;
        for (int i = 0; i < n; ++i)
            ans += (long) arr[i] * (i - left[i]) * (right[i] - i); // 累加贡献
        return ans % MOD;
    }

    int sumSubarrayMins_v2(vector<int> &arr) {
        int n = arr.size();
        vector<int> left(n, -1), right(n, n);
        stack<int> st;
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && arr[st.top()] >= arr[i]) {
                right[st.top()] = i; // i 恰好是栈顶的右边界
                st.pop();
            }
            if (!st.empty()) left[i] = st.top();
            st.push(i);
        }

        long ans = 0L;
        for (int i = 0; i < n; ++i)
            ans += (long) arr[i] * (i - left[i]) * (right[i] - i); // 累加贡献
        return ans % MOD;
    }

    int sumSubarrayMins(vector<int> &arr) {
        long ans = 0L;
        arr.push_back(-1);
        stack<int> st;
        st.push(-1); // 哨兵
        for (int r = 0; r < arr.size(); ++r) {
            while (st.size() > 1 && arr[st.top()] >= arr[r]) {
                int i = st.top();
                st.pop();
                ans += (long) arr[i] * (i - st.top()) * (r - i); // 累加贡献
            }
            st.push(r);
        }
        return ans % MOD;
    }

};