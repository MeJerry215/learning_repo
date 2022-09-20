#include "common.h"


/*
给定一个整数数组  nums 和一个正整数 k，找出是否有可能把这个数组分成 k 个非空子集，其总和都相等。
示例 1：
输入： nums = [4, 3, 2, 3, 5, 2, 1], k = 4
输出： True
说明： 有可能将其分成 4 个子集（5），（1,4），（2,3），（2,3）等于总和。
示例 2:
输入: nums = [1,2,3,4], k = 3
输出: false

提示：
1 <= k <= len(nums) <= 16
0 < nums[i] < 10000
每个元素的频率在 [1,4] 范围内
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/partition-to-k-equal-sum-subsets
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution {
public:
    bool dfsPartition(vector<int>& nums, int k, int i) {
        if (k == 0) {
            return true;
        }

        if (i == nums.size() || k < 0) {
            return false;
        }
        
        for(int j = i; j < nums.size(); j++) {
            if (nums[j] < 0) continue; 
            int num = nums[j];
            nums[j] = -1;
            print_vec(nums);
            if (dfsPartition(nums, k - num, j + 1)) {
                cout << "done" << endl;
                return true;
            }
            nums[j] = num;
        }
        return false;
    }

    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int tot = accumulate(nums.begin(), nums.end(), 0);
        if ((tot % k)) return false; 
        int avg = tot / k;
        int cnt = 0;
        cout << avg << endl;
        sort(nums.begin(), nums.end(), greater<int>());
        for(int i = 0; i < nums.size(); i++) {
            if (nums[i] < 0) { continue;}
            if (!dfsPartition(nums, avg, i)) {
                return false;
            }
            cnt ++;
        }
        return cnt == k;
    }
};



void TestSolution() {
    Solution s;
    vector<int> arr = {4,5,9,3,10,2,10,7,10,8,5,9,4,6,4,9};
    cout << s.canPartitionKSubsets(arr, 5) << endl;
}