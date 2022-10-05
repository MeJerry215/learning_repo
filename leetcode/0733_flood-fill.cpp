#include "common.h"


/*
有一幅以 m x n 的二维整数数组表示的图画 image ，其中 image[i][j] 表示该图画的像素值大小。
你也被给予三个整数 sr ,  sc 和 newColor 。你应该从像素 image[sr][sc] 开始对图像进行 上色填充 。
为了完成 上色工作 ，从初始像素开始，记录初始坐标的 上下左右四个方向上 像素值与初始坐标相同的相连像素点，接着再记录这四个方向上符合条件的像素点与他们对应 四个方向上 像素值与初始坐标相同的相连像素点，……，重复该过程。将所有有记录的像素点的颜色值改为 newColor 。
最后返回 经过上色渲染后的图像 。
 

示例 1:
输入: image = [[1,1,1],[1,1,0],[1,0,1]]，sr = 1, sc = 1, newColor = 2
输出: [[2,2,2],[2,2,0],[2,0,1]]
解析: 在图像的正中间，(坐标(sr,sc)=(1,1)),在路径上所有符合条件的像素点的颜色都被更改成2。
注意，右下角的像素没有更改为2，因为它不是在上下左右四个方向上与初始点相连的像素点。
示例 2:

输入: image = [[0,0,0],[0,0,0]], sr = 0, sc = 0, newColor = 2
输出: [[2,2,2],[2,2,2]]

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/flood-fill
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/



class Solution {
    constexpr static int directions[5] = {0, 1, 0, -1, 0};
public:
    /* 这道题目是基础的图的bfs遍历 */
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        vector<vector<bool>> visited(image.size(), vector<bool>(image[0].size(), false));
        queue<pair<int, int>> indexs;
        indexs.push({sr, sc});
        while(indexs.size()) {
            pair<int, int> index = indexs.front();
            indexs.pop();
            if (visited[index.first][index.second]) continue;
            visited[index.first][index.second] = true;
            int val = image[index.first][index.second];
            image[index.first][index.second] = color;
            for(int i =  1; i < 5; i++) {
                int x = index.first + directions[i - 1];
                int y = index.second + directions[i];
                if (x < 0 || y < 0 || x >= image.size() || y >= image[0].size() || image[x][y] != val) continue;
                indexs.push({x, y});
            }
        }
        return image;
    }
};