#include "common.h"


class Solution {
public:
    int GCD(int a, int b) {
        return b == 0 ? a : GCD(b, a % b);
    }

    int LCM(int a, int b) {
        return a * b / GCD(a, b);
    }

    int subarrayLCM(vector<int>& nums, int k) {
        int lcm = 1;
        int res = 0;
        for(int i = 0; i < nums.size(); i++) {
            lcm = nums[i];
            for(int j = i; j < nums.size(); j ++) {
                lcm = LCM(lcm, nums[j]);
                if (lcm > k) break;;
                if (lcm == k) res ++;
            }
        }


        return res;
    }
};

void TestSolution() {
    Solution s;
    vector<int> nums = {5,1,1,1,2};
    cout << s.subarrayLCM(nums, 1) << endl;

}

