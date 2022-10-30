#include "common.h"

class Solution {
public:
    vector<vector<string>> mostPopularCreator(vector<string>& creators, vector<string>& ids, vector<int>& views) {
        int n = creators.size();
        unordered_map<string, long long> view_cnts;
        unordered_map<string, pair<string, int>> max_views;
        long long max_view_cnt = 0;
        for(int i = 0; i < creators.size(); i++) {
            view_cnts[creators[i]] += views[i];
            max_view_cnt = max(max_view_cnt, view_cnts[creators[i]]);
            if (max_views[creators[i]].second < views[i]) {
                max_views[creators[i]].first = ids[i];
                max_views[creators[i]].second = views[i]; 
            } else if (max_views[creators[i]].second == views[i]) {
                if (max_views[creators[i]].first.empty()) {
                    max_views[creators[i]].first = ids[i];
                } else {
                    max_views[creators[i]].first = min(max_views[creators[i]].first, ids[i]);
                }
            }
        }
        vector<vector<string>> res;
        for(auto iter = view_cnts.begin(); iter != view_cnts.end(); iter++) {
            if (iter->second == max_view_cnt) {
                res.push_back({iter->first, max_views[iter->first].first});
            }
        }
        return res;
    }
};

void TestSolution() {
    Solution s;
}