#include "common.h"


/*
给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。
示例 1：
输入：height = [0,1,0,2,1,0,1,3,2,1,2,1]
输出：6
解释：上面是由数组 [0,1,0,2,1,0,1,3,2,1,2,1] 表示的高度图，在这种情况下，可以接 6 个单位的雨水（蓝色部分表示雨水）。 
示例 2：
输入：height = [4,2,0,3,2,5]
输出：9

提示：

n == height.length
1 <= n <= 2 * 104
0 <= height[i] <= 105

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/trapping-rain-water
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    int trap_v1(vector<int>& height) {
        /* 
        动态规划求解, 雨水高度取决于当前高度，左右两侧的最大高度，所以要寻找每个index的, lmax和rmax
        */
       if (height.size() < 3) return 0;
        vector<int> lmax(height.size(), 0);
        vector<int> rmax(height.size(), 0);
        lmax[0] = height[0];
        rmax[height.size() - 1] = height[height.size() - 1];
        for (int i = 1; i < height.size(); i++) {
            lmax[i] = max(height[i], lmax[i - 1]);
        }
        for(int i = height.size() - 2; i >= 0; i--) {
            rmax[i] = max(height[i], rmax[i + 1]);
        }
        int res = 0;
        for(int i = 0; i < height.size(); i++) {
            res += (min(lmax[i], rmax[i])  - height[i]);
        }
        return res;
    }
};