#include "common.h"


class Solution {
public:
    double largestSumOfAverages(vector<int>& nums, int k) {
        int n = nums.size();
        vector<double> prefix(n + 1);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
        vector<vector<double>> dp(k + 1, vector<double>(n + 1));
        for (int i = 1; i <= n; i++) {
            dp[1][i] = prefix[i] / i;
        }
        for (int j = 2; j <= k; j++) {
            for (int i = j; i <= n; i++) {
                for (int x = j - 1; x < i; x++) {
                    dp[j][i] = max(dp[j][i], dp[j - 1][x] + (prefix[i] - prefix[x]) / (i - x));
                }
            }
        }
        return dp[k][n];
    }

    double largestSumOfAverages(vector<int>& nums, int k) {
        int n = nums.size();
        vector<double> prefix(n + 1);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
        vector<double> dp(n + 1);
        for (int i = 1; i <= n; i++) {
            dp[i] = prefix[i] / i;
        }
        for (int j = 2; j <= k; j++) {
            for (int i = n; i >= j; i--) {
                for (int x = j - 1; x < i; x++) {
                    dp[i] = max(dp[i], dp[x] + (prefix[i] - prefix[x]) / (i - x));
                }
            }
        }
        return dp[n];
    }
};



void TestSolution() {
    Solution s;
    vector<int> nums = {
        4,1,7,5,6,2,3
    };
    cout << s.largestSumOfAverages(nums, 4) << endl;
}


