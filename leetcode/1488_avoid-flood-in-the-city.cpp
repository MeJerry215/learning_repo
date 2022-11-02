#include "common.h"

class Solution
{
public:
    vector<int> avoidFlood(vector<int> &rains)
    {
        unordered_set<int> pools;
        queue<int> sunny_q;
        vector<int> res(rains.size(), -1);
        for (int i = 0; i < rains.size(); i++)
        {
            if (rains[i] == 0)
            {
                sunny_q.push(i);
                continue;
            }
            if (pools.count(rains[i]))
            {
                if (sunny_q.size() > 0)
                {
                    res[sunny_q.front()] = rains[i];
                    sunny_q.pop();
                    pools.erase(rains[i]);
                }
                else
                {
                    return {};
                }
            }
            else
            {
                pools.insert(rains[i]);
            }
        }
        return res;
    }
};

class Solution
{
public:
    vector<int> avoidFlood(vector<int> &rains)
    {
        vector<int> ans(rains.size(), 1);
        unordered_map<int, int> water;
        set<int> zero;

        for (int i = 0; i < rains.size(); i++)
        {
            int r = rains[i];

            if (r == 0)
            {
                zero.insert(i);
                continue;
            }

            if (water.count(r) != 0)
            {
                auto it = zero.lower_bound(water[r]);
                if (it == zero.end())
                    return {};
                ans[*it] = r;
                zero.erase(it);
            }
            water[r] = i;
            ans[i] = -1;
        }

        return ans;
    }
};