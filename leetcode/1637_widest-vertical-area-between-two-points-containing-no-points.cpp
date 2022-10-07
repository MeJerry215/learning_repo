#include "common.h"

/*
给你 n 个二维平面上的点 points ，其中 points[i] = [xi, yi] ，请你返回两点之间内部不包含任何点的 最宽垂直面积 的宽度。
垂直面积 的定义是固定宽度，而 y 轴上无限延伸的一块区域（也就是高度为无穷大）。 最宽垂直面积 为宽度最大的一个垂直面积。
请注意，垂直区域 边上 的点 不在 区域内。

示例 1：
​
输入：points = [[8,7],[9,9],[7,4],[9,7]]
输出：1
解释：红色区域和蓝色区域都是最优区域。
示例 2：

输入：points = [[3,1],[9,0],[1,0],[1,4],[5,3],[8,8]]
输出：3

提示：
n == points.length
2 <= n <= 105
points[i].length == 2
0 <= xi, yi <= 109

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/widest-vertical-area-between-two-points-containing-no-points
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

/*
第一步： 先看题目，理解问题类型，和求解什么
相邻两点之间的 (xi, yi) 和 (xj, yj) 构成的矩形宽度(xj - xi) 高度 infinity的面积区域内 无其他点的存在
求可构成的矩形的最大宽度

第二步: 提示中的参数限制信息
题目中 点的数量 10^5 基本大概限制了题目只能被O(n)的时间复杂度求解，内存复杂度应该也要是一个常量级别的


第三步: 构造思路，明确考察类型
这道题目中应该要进行排序， 然后结果似乎跟y没有什么关系

第四步: 用例失败分析
NA

第五步: AC 分析
执行用时：
236 ms, 在所有 C++ 提交中击败了 78.49%的用户
内存消耗：
64.7 MB, 在所有 C++ 提交中击败了52.33%的用户
通过测试用例：
54 / 54
从结果最后运行耗时来看，这个一定是构造了一个特别大的测试用力，所以如果用了其他办法会执行超时
*/

class Solution {
public:
    int maxWidthOfVerticalArea(vector<vector<int>>& points) {
        sort(points.begin(), points.end(), [](const vector<int>& lhs, const vector<int>& rhs) {
            return lhs[0] < rhs[0];
        });
        int res = 0;
        for(int i = 1; i < points.size(); i++) {
            if (points[i][0] == points[i - 1][0]) {
                continue;
            }
            res = max(points[i][0] - points[i - 1][0], res);
        }
        return res;
    }
};