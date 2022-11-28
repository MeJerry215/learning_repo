#include "common.h"

class Solution {
public:
    bool increasingTriplet_v1(vector<int>& nums) {
        vector<pair<int,int>> values(nums.size(), {INT_MAX, INT_MIN});
        int value = INT_MAX;
        for(int i = 0; i < nums.size(); i++) {
            value = min(value, nums[i]);
            values[i].first = value;
        }
        value = INT_MIN;
        for(int i = nums.size() - 1; i >= 0; i--) {
            value = max(value, nums[i]);
            values[i].second = value;
        }
        for(int i = 1; i < nums.size() - 1; i++) {
            if (values[i].first < nums[i] && values[i].second > nums[i]) return true;
        }
        return false;
    }

    bool increasingTriplet(vector<int>& nums) {
        int one = nums[0], two = INT_MAX;
        for(int i = 1; i < nums.size(); i++) {
            if (nums[i] > two) return true;
            else if (nums[i] > one) two = nums[i];
            else one = nums[i];
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