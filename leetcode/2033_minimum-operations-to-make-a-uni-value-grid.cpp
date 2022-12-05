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

class Solution {
public:
    int minOperations(vector<vector<int>>& grd, int x) {
        int n = grd.size();
        int m = grd[0].size();
        vector<int> cnt;
        for(int i = 0; i < n; i ++){
            for(int j = 0; j < m; j ++){
                cnt.push_back(grd[i][j]);
            }
        }
        sort(cnt.begin(), cnt.end());
        int t = cnt.size()/ 2;
        int res = 0;
        for(int i = 0; i < cnt.size(); i ++){
            if(cnt[i] != cnt[t]){
                int k = abs(cnt[i] - cnt[t]);
                if(k % x  == 0) res += k / x;
                else return -1;
            }
        }
        return res;
    }
};
