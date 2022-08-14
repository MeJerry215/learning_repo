#include "common.h"

class Solution {
public:
    vector<int> sortedSquares_v1(vector<int>& nums) {
        sort(nums.begin(), nums.end(), [](int x, int y) {
            return abs(x) < abs(y);
        });
        vector<int> ans;
        for(int i = 0; i < nums.size(); i++) {
            ans.push_back(nums[i] * nums[i]);
        }
        return ans;
    }

    inline int abs_val(int value) {
        return (value ^ (value >> 31)) - (value >> 31);
    }

    vector<int> sortedSquares(vector<int>& nums) {
        
        vector<int> ans;
        int i = 0, j = nums.size() - 1;
        while(i <= j) {
            if (abs_val(nums[i]) > abs_val(nums[j])) {
                ans.push_back(nums[i] * nums[i++]);
            } else {
                ans.push_back(nums[j] * nums[j--]);
            }
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }
};

void TestSolution() {
    Solution s;
    int x = -1;
    cout << s.abs_val(-1) << endl;
}