#include "common.h"

class Solution
{
public:
    vector<vector<int>> merge(vector<vector<int>> &intervals) {
        sort(intervals.begin(), intervals.end(), [](vector<int>& v1, vector<int>& v2) {
            return v1[0] == v2[0] ?  v1[1] < v2[1] : v1[0] < v2[0];
        });
        vector<vector<int>> merged;
        for(auto& interval: intervals) {
            if (merged.size() == 0) {
                merged.push_back(interval);
            }
            auto& back = merged.back();
            if (back[1] < interval[0]) {
                merged.push_back(interval);
            } else {
                back[1] = max(interval[1], back[1]);
            }

        }
        return merged;
    }
};