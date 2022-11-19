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

class Solution {
public:
    bool carPooling(vector<vector<int>>& trips, int capacity) {
        map<int, int> m;
        for(auto & t: trips){
            m[t[1]] += t[0];  // pick up
            m[t[2]] -= t[0];  // drop off
        }
        int passengers = 0;
        for(auto it: m){
            passengers += it.second;
            if(passengers > capacity) return false;
        }
        return true;
    }
};