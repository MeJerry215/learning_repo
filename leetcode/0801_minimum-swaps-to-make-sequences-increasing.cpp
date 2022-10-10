#include "common.h"

class Solution
{
public:
    int minSwap(vector<int> &nums1, vector<int> &nums2)
    {
        // 二值动态规划, dp[i][0] 表示当前位置不互换的最小操作次数， dp[i][1] 表示当前位置互换的最小操作次数。
        int dp[nums1.size()][2];
        dp[0][0] = 0;
        dp[0][1] = 1;
        for(int i = 1; i < nums1.size(); i++) {
            /*
            由于当前互换仅仅和前面元素的互换结果相关, 所以所以仅看 nums1[i], nums2[i], nums1[i-1], nums2[i-1]
            条件1 nums1[i] > nums1[i - 1] && nums2[i] > nums2[i - 1]
            条件2 nums1[i] > nums2[i - 1] && nums2[i] > nums1[i - 1]
            case 1 当前位置不跟随前面元素的的结果变化而变化, 即 同时满足了条件1和条件2
            1 3     dp[i][0] = min(dp[i - 1][0], dp[i - 1][1])
            2 4     dp[i][1] = dp[i][0] + 1
            case 2 当前位置随之前元素的位置互换结果发生了改变, 即只满足条件1, 所以dp[i][0]必须不互换
            1 2     dp[i][0] = dp[i-1][0]
            3 4     dp[i][1] = dp[i-1][1] + 1
            case 3  无论如何都必须互换i 或者i - 1
            1 4     dp[i][0] = dp[i - 1][1]  如果i【不互换】，则i-1必须【互换】
            3 2     dp[i][1] = dp[i - 1][0] + 1  如果i【互换】，则i-1必须【不互换】
            */
            if (nums1[i] > nums1[i-1] && nums2[i] > nums2[i-1]) {
                if (nums1[i] > nums2[i - 1] && nums2[i] > nums1[i - 1]) {
                    dp[i][0] = min(dp[i - 1][0], dp[i - 1][1]);
                    dp[i][1] = dp[i][0] + 1;
                } else {
                    dp[i][0] = dp[i-1][0];
                    dp[i][1] = dp[i-1][1] + 1;
                }
            } else {
                dp[i][0] = dp[i - 1][1];
                dp[i][1] = dp[i - 1][0] + 1;
            }
        }
        return min(dp[nums1.size() - 1][0], dp[nums1.size() - 1][1]);
    }
};