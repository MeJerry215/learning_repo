#include "common.h"

class Solution
{
public:
    int minimumRounds(vector<int> &tasks)
    {
        unordered_map<int, int> task_count;
        for (auto task : tasks)
        {
            task_count[task]++;
        }
        int res = 0;
        for (auto iter : task_count)
        {
            if (iter.second == 1) return -1;
            res = res + iter.second / 3 + (iter.second % 3 > 0);
        }
        return res;
    }
};