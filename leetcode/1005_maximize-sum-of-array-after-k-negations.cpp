#include "common.h"

class Solution {
public:
    int largestSumAfterKNegations(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end(), less<int>());
        int min_val = INT_MAX;
       for(int i = 0; i < nums.size(); i ++) {
            if(nums[i] < 0 && k > 0) {
                nums[i] = -nums[i];
                k--;
            }
            min_val = min(min_val, nums[i]);
       }
       int sum = accumulate(nums.begin(), nums.end(), 0);
        return k % 2 == 0 ? sum : sum - 2 * min_val;

    }
};

void TestSolution() {
    vector<int> nums = {3,-1,0,2};
    Solution s;
    cout << s.largestSumAfterKNegations(nums, 2) << endl;
}