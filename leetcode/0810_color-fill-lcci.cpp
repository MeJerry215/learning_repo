#include "common.h"

/*
编写函数，实现许多图片编辑软件都支持的「颜色填充」功能。
待填充的图像用二维数组 image 表示，元素为初始颜色值。初始坐标点的行坐标为 sr 列坐标为 sc。需要填充的新颜色为 newColor 。
「周围区域」是指颜色相同且在上、下、左、右四个方向上存在相连情况的若干元素。
请用新颜色填充初始坐标点的周围区域，并返回填充后的图像。

示例：
输入：
image = [[1,1,1],[1,1,0],[1,0,1]]
sr = 1, sc = 1, newColor = 2
输出：[[2,2,2],[2,2,0],[2,0,1]]
解释:
初始坐标点位于图像的正中间，坐标 (sr,sc)=(1,1) 。
初始坐标点周围区域上所有符合条件的像素点的颜色都被更改成 2 。
注意，右下角的像素没有更改为 2 ，因为它不属于初始坐标点的周围区域。

提示：
image 和 image[0] 的长度均在范围 [1, 50] 内。
初始坐标点 (sr,sc) 满足 0 <= sr < image.length 和 0 <= sc < image[0].length 。
image[i][j] 和 newColor 表示的颜色值在范围 [0, 65535] 内。

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/color-fill-lcci
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution
{
public:
    int directions[5] = {0, 1, 0, -1, 0};

    vector<vector<int>> floodFill(vector<vector<int>> &image, int sr, int sc, int newColor)
    {
        int srcColor = image[sr][sc];
        queue<pair<int, int>> indexs;
        indexs.emplace(sr, sc);
        int m = image.size();
        int n = image[0].size();
        if (srcColor == newColor) return image;
        auto check_valid = [&](int x, int y) -> bool
        {
            return !(x < 0 || y < 0 || x >= m || y >= n);
        };
        while(!indexs.empty()) {
          pair<int, int>& index = indexs.front();
          image[index.first][index.second] = newColor;
          for(int i = 0; i < 4; i++) {
            int x = index.first + directions[i];
            int y = index.second + directions[i + 1];
            if (check_valid(x, y) && image[x][y] == srcColor) {
                indexs.emplace(x, y);
            }
          }
          indexs.pop();
        }
        return image;
    }
};
