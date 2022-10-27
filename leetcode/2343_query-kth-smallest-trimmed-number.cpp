#include "common.h"

class Solution
{
public:
    vector<int> smallestTrimmedNumbers(vector<string> &nums, vector<vector<int>> &queries)
    {
        vector<int> indexs(nums.size(), 0);
        vector<int> res;
        int n = nums[0].size();
        for (vector<int> &query : queries)
        {
            int mask = n - query[1];
            iota(indexs.begin(), indexs.end(), 0);
            stable_sort(indexs.begin(), indexs.end(), [&](int i, int j)
            {
                for (int k = mask; k < n; k++) {
                    if (nums[i][k] == nums[j][k]) continue;
                    return nums[i][k] < nums[j][k];
                }
                return false;
            });
            res.push_back(indexs[query[0] - 1]);
        }
        return res;
    }
};