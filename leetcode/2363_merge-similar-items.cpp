#include "common.h"


class Solution {
public:
    vector<vector<int>> mergeSimilarItems(vector<vector<int>>& items1, vector<vector<int>>& items2) {
        unordered_map<int, int> weights;
        for(auto& item: items1) {
            weights[item[0]] += item[1];

        }
        for(auto& item: items2) {
            weights[item[0]] += item[1];

        }
        vector<vector<int>> res;
        for(auto& weight: weights) {
            res.push_back({weight.first, weight.second});

        }
        sort(res.begin(), res.end(), [&](const auto& lhs, const auto& rhs) {
            return lhs[0] < rhs[0];

        });
        return res;

    }
};