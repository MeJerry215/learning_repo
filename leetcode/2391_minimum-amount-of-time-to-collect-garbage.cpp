#include "common.h"


class Solution {
public:
    int garbageCollection(vector<string>& garbage, vector<int>& travel) {
        vector<int> travel_sum;
        travel_sum.push_back(0);
        for (auto t : travel) {
            travel_sum.push_back(travel_sum.back() + t);
        }
        int res = 0;
        unordered_map<char, int> last_pos = {{'M', 0}, {'G', 0}, {'P', 0}};
        for (int i = 0; i < garbage.size(); i++) {
            string& str = garbage[i];
            for (char c: str) {
                res ++;
                if (last_pos[c] != i) {
                    res += travel_sum[i] - travel_sum[last_pos[c]];
                    last_pos[c] = i;
                }
            }
        }
        return res;
    }
};