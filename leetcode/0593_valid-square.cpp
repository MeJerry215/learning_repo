#include "common.h"

class Solution
{
public:
    int cal_distance(vector<int> &p1, vector<int> &p2)
    {
        return (p2[0] - p1[0]) * (p2[0] - p1[0]) + (p2[1] - p1[1]) * (p2[1] - p1[1]);
    }

    bool validSquare(vector<int> &p1, vector<int> &p2, vector<int> &p3, vector<int> &p4)
    {
        unordered_map<int, int> edges;
        edges[cal_distance(p1, p2)]++;
        edges[cal_distance(p1, p3)]++;
        edges[cal_distance(p1, p4)]++;
        edges[cal_distance(p2, p3)]++;
        edges[cal_distance(p2, p4)]++;
        edges[cal_distance(p3, p4)]++;
        if (edges.size() > 2)
            return false;
        int t1 = 0, t2 = 0;
        for (auto iter : edges)
        {
            if (iter.second == 4)
            {
                t1 = iter.first;
            }
            else if (iter.second == 2)
            {
                t2 = iter.first;
            }
            else
            {
                return false;
            }
        }
        return t1 < t2;
    }
};