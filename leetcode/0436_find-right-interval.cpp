#include "common.h"

/**
给你一个区间数组 intervals ，其中 intervals[i] = [starti, endi] ，且每个 starti 都 不同 。
区间 i 的 右侧区间 可以记作区间 j ，并满足 startj >= endi ，且 startj 最小化 。
返回一个由每个区间 i 的 右侧区间 的最小起始位置组成的数组。如果某个区间 i 不存在对应的 右侧区间 ，则下标 i 处的值设为 -1 。

示例 1：
输入：intervals = [[1,2]]
输出：[-1]
解释：集合中只有一个区间，所以输出-1。
示例 2：

输入：intervals = [[3,4],[2,3],[1,2]]
输出：[-1,0,1]
解释：对于 [3,4] ，没有满足条件的“右侧”区间。
对于 [2,3] ，区间[3,4]具有最小的“右”起点;
对于 [1,2] ，区间[2,3]具有最小的“右”起点。
示例 3：

输入：intervals = [[1,4],[2,3],[3,4]]
输出：[-1,2,-1]
解释：对于区间 [1,4] 和 [3,4] ，没有满足条件的“右侧”区间。
对于 [2,3] ，区间 [3,4] 有最小的“右”起点。

提示：
1 <= intervals.length <= 2 * 104
intervals[i].length == 2
-106 <= starti <= endi <= 106
每个间隔的起点都 不相同
 */
class Solution
{
public:
    vector<int> findRightInterval(vector<vector<int>> &intervals)
    {
        vector<pair<int, int>> startIntervals;
        for (int i = 0; i < intervals.size(); i++)
        {
            startIntervals.push_back(make_pair(intervals[i][0], i));
        }
        sort(startIntervals.begin(), startIntervals.end());
        vector<int> ans(intervals.size(), -1);
        for (int i = 0; i < intervals.size(); i++)
        {
            // lower_bound 要求输入排序后数列，内部使用二分法搜索
            // 找到end >= start 可以插入的地方, 0保证总是在最前插入在，相比之前得到的index。
            auto it = lower_bound(startIntervals.begin(), startIntervals.end(), make_pair(intervals[i][1], 0));
            if (it != startIntervals.end())
            {
                ans[i] = it->second;
            }
        }
        return ans;
    }
};

void TestSolution()
{
    Solution s;
    vector<vector<int>> intervals = {
        {3, 4}, {2, 3}, {1, 2}};
    auto ans = s.findRightInterval(intervals);
    print_vec(ans);
    intervals = {
        {1, 4}, {2, 3}, {3, 4}};
    ans = s.findRightInterval(intervals);
    print_vec(ans);
}