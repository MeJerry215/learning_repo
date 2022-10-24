#include "common.h"


class Solution {
public:
    long long makeSimilar(vector<int>& nums, vector<int>& target) {
        sort(nums.begin(), nums.end());
        sort(target.begin(), target.end());
        int odd_idx= 0, even_idx = 0;
        int idx[2] = {0, 0};
        long long res = 0;
        for (int i = 0; i < nums.size(); i++) {
            bool is_odd = nums[i] % 2;
            while(target[idx[is_odd]] % 2 != is_odd) 
                idx[is_odd]++;
            res += abs(nums[i] - target[idx[is_odd]]);

            idx[is_odd]++;
        }
        return res / 4;
    }
};