#include "common.h"

class Solution {
public:
    vector<int> getAverages(vector<int>& nums, int k) {
    if (k == 0) return nums;
    vector<int> res(nums.size(), -1);
    if (2 * k + 1 > nums.size()) return res;
    long long sum = 0;

    for(int i = 0; i < 2 * k + 1; i++) {
      sum += nums[i];
    }
    cout << sum << endl;
    res[k] = sum / (2 * k + 1);
    for(int i = k + 1; i < nums.size() - k; i++) {
        sum -= nums[i - k - 1];
        sum += nums[i + k];
        res[i] = sum / (2 * k + 1);
    }
    return res;
  }
};