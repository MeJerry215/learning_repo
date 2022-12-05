#include "common.h"

class Solution
{
public:
    int minJumps(vector<int> &arr)
    {
        unordered_map<int, unordered_set<int>> caches;
        for (int i = 0; i < arr.size(); i++)
        {
            caches[arr[i]].insert(i);
        }
        int step = 0;
        queue<int> postions;
        vector<bool> visited(arr.size(), false);
        postions.push(0);
        arr[0] = true;
        while (!postions.empty())
        {
            int n = postions.size();
            cout << "step " << step << endl;
            
            for (int i = 0; i < n; i++)
            {
                int pos = postions.front();
                if (pos == arr.size() - 1)
                    return step;
                postions.pop();
                cout << "\tvisit " << pos << endl;
                // visited
                if (pos - 1 > 0 && !visited[pos - 1]) {
                    postions.push(pos - 1);
                    visited[pos - 1] = true;
                }
                if (pos + 1 < arr.size() && !visited[pos + 1]) {
                    postions.push(pos + 1);
                    visited[pos + 1] = true;
                }
                if (!caches[arr[pos]].empty()) {
                    caches[arr[pos]].erase(pos);
                    for (auto iter : caches[arr[pos]])
                    {
                        if (visited[iter])
                            continue;
                        postions.push(iter);
                        visited[iter] = true;
                    }
                }
            }
            step++;
        }
        return step;
    }
};