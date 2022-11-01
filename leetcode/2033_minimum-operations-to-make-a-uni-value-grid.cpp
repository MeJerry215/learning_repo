#include "common.h"

class Solution
{
public:
    int minOperations(vector<vector<int>> &grid, int k)
    {
        int m = grid.size();
        int n = grid[0].size();
        int n_elem = m * n;
        vector<int> idxs(n_elem, 0);
        iota(idxs.begin(), idxs.end(), 0);
        sort(idxs.begin(), idxs.end(), [&](int lhs, int rhs)
             { return grid[lhs / n][lhs % n] < grid[rhs / n][rhs % n]; });
        int idx = idxs[n_elem / 2];
        int median = grid[idx / n][idx % n];
        int res = 0;
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int diff = abs(grid[i][j] - median);
                if (diff % k)
                    return -1;
                res += diff / k;
            }
        }
        return res;
    }
};