#include "common.h"


class Solution {
public:
    int countSubarrays(vector<int>& nums, int k) {
        auto iter = find(nums.begin(), nums.end(), k);
        if (iter == nums.end()) return 0;
        unordered_map<int, int> cnts;
        int pos = iter - nums.begin();
        int less_cnt = 0, greater_cnt = 0;
        int i = pos;
        while(i < nums.size()) {
            if (nums[i] > k) greater_cnt++;
            else if (nums[i] < k) less_cnt++;
            i++;
            cnts[greater_cnt - less_cnt] ++; 
        }
        
        int res = 0;
        i = pos, greater_cnt = 0, less_cnt = 0;
        while(i >= 0) {
            if (nums[i] > k) greater_cnt++;
            else if (nums[i] < k) less_cnt++;
            i--;
            res += cnts[less_cnt - greater_cnt];
            res += cnts[1 + less_cnt - greater_cnt];
        }
        return res;

    }
};

void TestSolution() {
    Solution s;
    vector<int> nums = {9, 1, 8, 2, 5, 6, 3, 4, 7};
    cout << s.countSubarrays(nums, 5) << endl;
}