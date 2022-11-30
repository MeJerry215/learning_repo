#include "common.h"


/*
在一排多米诺骨牌中，A[i] 和 B[i] 分别代表第 i 个多米诺骨牌的上半部分和下半部分。（一个多米诺是两个从 1 到 6 的数字同列平铺形成的 —— 该平铺的每一半上都有一个数字。）

我们可以旋转第 i 张多米诺，使得 A[i] 和 B[i] 的值交换。

返回能使 A 中所有值或者 B 中所有值都相同的最小旋转次数。

如果无法做到，返回 -1.

 

示例 1：



输入：A = [2,1,2,4,2,2], B = [5,2,6,2,3,2]
输出：2
解释：
图一表示：在我们旋转之前， A 和 B 给出的多米诺牌。
如果我们旋转第二个和第四个多米诺骨牌，我们可以使上面一行中的每个值都等于 2，如图二所示。
示例 2：

输入：A = [3,5,1,2,3], B = [3,6,3,3,4]
输出：-1
解释：
在这种情况下，不可能旋转多米诺牌使一行的值相等。
 

提示：

1 <= A[i], B[i] <= 6
2 <= A.length == B.length <= 20000

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/minimum-domino-rotations-for-equal-row
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    int check(vector<int>& top, vector<int>& bottom, int val) {
        int top_swap = 0, bottom_swap = 0;
        for(int i = 0; i < top.size(); i++) {
            if (top[i] != val && bottom[i] != val) return -1;
            if (top[i] != val) top_swap ++;
            if (bottom[i] != val) bottom_swap ++;
        }
        return min(top_swap, bottom_swap);
    }

    int minDominoRotations_v1(vector<int>& tops, vector<int>& bottoms) {
        int val = check(tops, bottoms, tops[0]);
        if (val != -1 || tops[0] == bottoms[0]) return val;
        return check(tops, bottoms, bottoms[0]);
    }

    int minDominoRotations(vector<int>& tops, vector<int>& bottoms) {
        int top_cnt[7]{}, bottom_cnt[7]{}, num_cnt[7];
        for(int i = 0; i < 7; i++) num_cnt[i] = tops.size();
        for(int i = 0; i < tops.size(); i++) {
            if (tops[i] == bottoms[i]) {
                num_cnt[tops[i]]--;
            } else {
                top_cnt[tops[i]]++;
                bottom_cnt[bottoms[i]] ++;
            }
        }
        for(int i = 1; i < 7; i++) {
            if ((top_cnt[i] + bottom_cnt[i]) == num_cnt[i]) return min(top_cnt[i], bottom_cnt[i]);
        }
        return -1;
    }
};