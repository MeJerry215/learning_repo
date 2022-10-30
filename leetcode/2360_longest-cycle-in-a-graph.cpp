#include "common.h"


class Solution
{
public:
    int longestCycle(vector<int> &edges)
    {
        int res = -1;
        // vector<bool> visited(edges.size(), false);
        for (int i = 0; i < edges.size(); i++)
        {
            // if (visited[i] == fal) continue;
            if (edges[i] == -1)
                continue;
            int slow = i;
            int fast = edges[slow];
            while (slow != fast)
            {
                // edges[low] = 0;
                if (slow == -1 || fast == -1)
                    break;
                slow = edges[slow];
                fast = edges[fast];
                if (fast == -1)
                    break;
                fast = edges[fast];
            }
            /* meet cycle*/
            if (slow == fast && slow != -1)
            {
                int start = slow;
                int end = edges[start];
                int len = 1;
                while (start != end)
                {
                    end = edges[end];
                    len++;
                }
                res = max(res, len);
            }
            /* paint the cycle */
            slow = i;
            while (edges[slow] != -1)
            {
                int next = edges[slow];
                edges[slow] = -1;
                slow = next;
            }
        }
        return res;
    }

    int longestCycle(vector<int> &edges) {
        int n = edges.size(), time[n], ans = -1;
        memset(time, 0, sizeof(time));
        for (int i = 0, clock = 1; i < n; ++i) {
            if (time[i]) continue;
            for (int x = i, start_time = clock; x >= 0; x = edges[x]) {
                if (time[x]) { // 重复访问
                    if (time[x] >= start_time) // 找到了一个新的环
                        ans = max(ans, clock - time[x]);
                    break;
                }
                time[x] = clock++;
            }
        }
        return ans;
    }

};