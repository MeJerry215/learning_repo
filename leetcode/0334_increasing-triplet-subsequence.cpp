#include "common.h"

class Solution {
public:
    bool increasingTriplet(vector<int>& nums) {
        int cnt = 1;
        for(int i = 1; i < nums.size(); i++) {
            if (nums[i] > nums[i - 1]) {
                cnt += 1;
                if (cnt == 3) return true;
            } else {
                cnt = 1;
            }
        }
        return false;
    }
};

void TestSolution() {
    Solution s;
    vector<int> nums = {
        5, 4, 3, 2, 1
    };
    cout << s.increasingTriplet(nums) << endl;
}