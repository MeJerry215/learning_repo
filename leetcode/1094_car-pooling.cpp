#include "common.h"

class Solution {
public:
    bool carPooling(vector<vector<int>>& trips, int capacity) {
        priority_queue<pair<int, int>, vector<pair<int,int>>, greater<pair<int,int>>> toleave;
        int tot = 0;
        sort(trips.begin(), trips.end(), [](vector<int>& lhs,  vector<int>& rhs) {
            return lhs[1] < rhs[1];
        });
        for(auto& trip: trips) {
            while (!toleave.empty() && toleave.top().first < trip[1]) {
                tot -= toleave.top().second;
                toleave.pop();
            }
            tot += trip[0];
            if (tot > capacity) return false;
            toleave.emplace(trip[2], trip[0]);
        }
        return true;
    }
};


[[2,1,5],[3,5,7]]
3