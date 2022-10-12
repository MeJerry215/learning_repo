#include "common.h"

/*
矩形以列表 [x1, y1, x2, y2] 的形式表示，其中 (x1, y1) 为左下角的坐标，(x2, y2) 是右上角的坐标。矩形的上下边平行于 x 轴，左右边平行于 y 轴。
如果相交的面积为 正 ，则称两矩形重叠。需要明确的是，只在角或边接触的两个矩形不构成重叠。
给出两个矩形 rec1 和 rec2 。如果它们重叠，返回 true；否则，返回 false 。

 

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/rectangle-overlap
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

lass Solution {
public:
    bool isRectangleOverlap(vector<int>& rec1, vector<int>& rec2) {
        return (min(rec1[2], rec2[2]) > max(rec1[0], rec2[0]) &&
                min(rec1[3], rec2[3]) > max(rec1[1], rec2[1]));
    }
};
