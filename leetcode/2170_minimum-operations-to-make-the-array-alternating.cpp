#include "common.h"


class Solution {
public:
    int minimumOperations(vector<int>& nums) {
        int n = nums.size();

        // start = 0 表示偶数下标，start = 1 表示奇数下标
        // 返回值为最大键，最大键对应的值，次大键，次大键对应的值
        auto get = [&](int start) -> tuple<int, int, int, int> {
            unordered_map<int, int> freq;
            for (int i = start; i < n; i += 2) {
                ++freq[nums[i]];
            }

            int fstkey = 0, fstval = 0, sndkey = 0, sndval = 0;
            for (const auto& [key, val]: freq) {
                if (val > fstval) {
                    tie(sndkey, sndval) = tuple{fstkey, fstval};
                    tie(fstkey, fstval) = tuple{key, val};
                }
                else if (val > sndval) {
                    tie(sndkey, sndval) = tuple{key, val};
                }
            }

            return {fstkey, fstval, sndkey, sndval};
        };

        auto [e1stkey, e1stval, e2ndkey, e2ndval] = get(0);
        auto [o1stkey, o1stval, o2ndkey, o2ndval] = get(1);

        if (e1stkey != o1stkey) {
            return n - (e1stval + o1stval);
        }
        
        return n - max(e1stval + o2ndval, o1stval + e2ndval);
    }
};