#include "common.h"



class Solution {
public:
    long long countSubarrays(vector<int>& nums, int minK, int maxK) {
        /*
        1   1   1   3   5   2   2   5   7   1   2    minK = 1, maxK = 5
        1   1   (1   3   5)   2   2   5   7   1   2    ... 对于任意数组 假如存在一个满足的minK, maxK条件的最小子数组, 则其往左能够选择的数, 以及往右依旧有效的数在本例中为，其左右为minK和maxK
       (1   1   (1   3   5)   2   2   5)  7   1   2
        实际能够产生
        1   1   1   3   5
            1   1   3   5
                1   3   5
                1   3   5   2
                1   3   5   2   2
                1   3   5   2   2   5
            1   1   3   5   2
            1   1   3   5   2   2
            1   1   3   5   2   2   5
        1   1   3   5   2   2   5   5
        1   1   3   5   2   2   2
        1   1   3   5   2   2   2   5  12 种   = 3 * 4, 即最小数组左右 可以沿生有效长度
        
        1  (1   5)  5   1   5   7    考虑一个这个场景，在我们找到的第一个符合区间的 已经搜索到了 2 * 4 = 8
        1   1   (5  (5   1)  5) 7   2 * 2 = 4 则产生的
        1   1   5   5   (1  5)  7   1


        (1)   1   1   1     1 * 4
        1   (1)   1   1     1 * 3
        1   1   (1)   1     1 * 2
        1   1   1   (1)     1 * 1
        */

       /* 找到符合条件的left和right的边界  */
       int left = 0;
       int cur_min = nums[0], cur_max = nums[0];
       for(int i = 1; i < nums.size(); i++) {
            cout << "i " <<i << " " << (i == nums.size() -1) << endl;

            if (nums[i] > maxK || nums[i] < minK || i == nums.size() - 1) {
                cout << "ok range " << left << " " << i - 1 << endl;
                left = i + 1;
            }

       }
        return 0;
    }
};

void TestSolution() {
    Solution s;
    vector<int> nums = {1, 1, 5, 5, 1, 5, 7, 1, 2};
    s.countSubarrays(nums, 1, 5);
}