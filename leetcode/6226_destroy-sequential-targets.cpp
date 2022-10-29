#include "common.h"


class Solution {
public:
    int destroyTargets(vector<int>& nums, int space) {
        unordered_map<int, pair<int,int>> hash_space;
        int res = INT_MAX;
        int max_cnt = 0;
        for(auto num: nums) {
            int bucket_idx = num % space;
            if (hash_space.count(bucket_idx)) {
                hash_space[bucket_idx].second++;
                hash_space[bucket_idx].first = min(hash_space[bucket_idx].first, num);
            } else {
                hash_space.emplace(bucket_idx, make_pair(num, 1));
            }

            if (hash_space[bucket_idx].second > max_cnt) {
                max_cnt = hash_space[bucket_idx].second;
                res = hash_space[bucket_idx].first;
            } else if (hash_space[bucket_idx].second == max_cnt) {
                res = min(res, hash_space[bucket_idx].first);
            }

        }
        return res;
    }
};



void TestSolution() {
    vector<int> nums = {1,3,5,2,4,6};
    int space = 2;
    Solution s;
    cout << s.destroyTargets(nums, space) << endl;

}