#include "common.h"

class Solution
{
public:
    vector<vector<int>> kClosest(vector<vector<int>> &points, int k)
    {
        sort(points.begin(), points.end(), [](const vector<int> &lhs, const vector<int> &rhs)
             { return (lhs[0] * lhs[0] + lhs[1] * lhs[1]) < (rhs[0] * rhs[0] + rhs[1] * rhs[1]); });
        points.resize(k);
        return points;
    }
};