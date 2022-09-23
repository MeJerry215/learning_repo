#include "common.h"

class Solution
{
public:
    int maxCount(int m, int n, vector<vector<int>> &ops)
    {
        int min_m = m, min_n = n;
        for (const vector<int> &op : ops)
        {
            min_m = min(op[0], min_m);
            min_n = min(op[1], min_n);
        }
        return min_m * min_n;
    }
};