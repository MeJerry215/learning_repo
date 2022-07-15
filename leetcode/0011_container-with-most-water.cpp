#include "common.h"


/*
给定一个长度为 n 的整数数组 height 。有 n 条垂线，第 i 条线的两个端点是 (i, 0) 和 (i, height[i]) 。
找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。
返回容器可以储存的最大水量。
说明：你不能倾斜容器。

 

示例 1：
输入：[1,8,6,2,5,4,8,3,7]
输出：49 
解释：图中垂直线代表输入数组 [1,8,6,2,5,4,8,3,7]。在此情况下，容器能够容纳水（表示为蓝色部分）的最大值为 49。
示例 2：

输入：height = [1,1]
输出：1
提示：
n == height.length
2 <= n <= 105
0 <= height[i] <= 104

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/container-with-most-water
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution {
public:
    int maxArea_v1(vector<int>& height) {
        int max_val = 0;
        for(int i = 0; i < height.size(); i++) {
            for (int j = i + 1; j < height.size(); j++) {
                max_val = max(min(height[i], height[j]) * (j - i), max_val);
            }
        }
        return max_val;
    }

    int maxArea(vector<int>& height) {
        int max_val = 0;
        int i = 0, j = height.size() - 1;
        while(i < j) {
            max_val = max(max_val, (j - i) * min(height[i], height[j]));
            height[i] < height[j] ? i++ : j--;
        }
        return max_val;
    }
};

void TestSolution() {
    Solution s;
    vector<int> height = {1,8,6,2,5,4,8,3,7};
    cout << s.maxArea(height) << endl;
}