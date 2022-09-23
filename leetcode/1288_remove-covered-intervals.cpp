#include "common.h"

/*
the left points have been sorted in ascend order. there is no need to check the left margin of the range,
and right marigin is in descend order which means if follow points's right margin less than the right max value, it's native in the range.
when meet larger right marigin update it and continue to find removed node.
so the only thing to solve this is to check weather the following right marigin of following point is less than the r/
take below sorted array as example
1  56    update r_max = 56
1  40    rm_cnt ++
1  10    rm_cnt ++
2  30    rm_cnt ++
30 40    rm_cnt ++
56 70    update r_max = 70
57 80    update r_max = 80
57 70    rm_cnt ++
*/
class Solution
{
public:
    int removeCoveredIntervals(vector<vector<int>> &intervals)
    {
        sort(intervals.begin(), intervals.end(), [](const vector<int> &lhs, const vector<int> &rhs)
             { return lhs[0] < rhs[0] || (lhs[0] == rhs[0] && lhs[1] > rhs[1]); });
        int r_max = intervals[0][1];
        int rm_cnt = 0;
        for (int i = 1; i < intervals.size(); i++) {
            if (intervals[i][1] <= r_max)
            {
                rm_cnt++;
            }
            else
            {
                r_max = intervals[i][1];
            }
        }
        return intervals.size() - rm_cnt;
    }
};