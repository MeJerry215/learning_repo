#include "common.h"


/*
给你一份 n 位朋友的亲近程度列表，其中 n 总是 偶数 。
对每位朋友 i，preferences[i] 包含一份 按亲近程度从高到低排列 的朋友列表。换句话说，排在列表前面的朋友与 i 的亲近程度比排在列表后面的朋友更高。每个列表中的朋友均以 0 到 n-1 之间的整数表示。
所有的朋友被分成几对，配对情况以列表 pairs 给出，其中 pairs[i] = [xi, yi] 表示 xi 与 yi 配对，且 yi 与 xi 配对。
但是，这样的配对情况可能会使其中部分朋友感到不开心。在 x 与 y 配对且 u 与 v 配对的情况下，如果同时满足下述两个条件，x 就会不开心：
x 与 u 的亲近程度胜过 x 与 y，且
u 与 x 的亲近程度胜过 u 与 v
返回 不开心的朋友的数目 。

示例 1：
输入：n = 4, preferences = [[1, 2, 3], [3, 2, 0], [3, 1, 0], [1, 2, 0]], pairs = [[0, 1], [2, 3]]
输出：2
解释：
朋友 1 不开心，因为：
- 1 与 0 配对，但 1 与 3 的亲近程度比 1 与 0 高，且
- 3 与 1 的亲近程度比 3 与 2 高。
朋友 3 不开心，因为：
- 3 与 2 配对，但 3 与 1 的亲近程度比 3 与 2 高，且
- 1 与 3 的亲近程度比 1 与 0 高。
朋友 0 和 2 都是开心的。
示例 2：
输入：n = 2, preferences = [[1], [0]], pairs = [[1, 0]]
输出：0
解释：朋友 0 和 1 都开心。
示例 3：
输入：n = 4, preferences = [[1, 3, 2], [2, 3, 0], [1, 3, 0], [0, 2, 1]], pairs = [[1, 3], [0, 2]]
输出：4

提示：
2 <= n <= 500
n 是偶数
preferences.length == n
preferences[i].length == n - 1
0 <= preferences[i][j] <= n - 1
preferences[i] 不包含 i
preferences[i] 中的所有值都是独一无二的
pairs.length == n/2
pairs[i].length == 2
xi != yi
0 <= xi, yi <= n - 1
每位朋友都 恰好 被包含在一对中
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/count-unhappy-friends
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/


class Solution {
public:
    int unhappyFriends(int n, vector<vector<int>>& preferences, vector<vector<int>>& pairs) {
        /*
        题目逻辑比较绕，属于一道逻辑题?
        1. 由于题目给出来的是任意x y 亲近顺序，所以首先第一步增加一个辅助矩阵来获取亲近顺序上的人的
        2. 转换配对 x和y配对以及y和x的配对关系。
        3. 找到亲密度更高的人，然后查看其亲密度
        */
        vector<vector<int>> order(n, vector<int>(n));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n - 1; ++j) {
                // 这里的order起始是任意两个朋友之间的亲密度，j越小亲密度越高
                order[i][preferences[i][j]] = j;
            }
        }
        vector<int> match(n);
        for (const auto& pr: pairs) {
            match[pr[0]] = pr[1];
            match[pr[1]] = pr[0];
        }

        int unhappyCount = 0;
        for (int x = 0; x < n; ++x) {
            int y = match[x];
            // 获得x y 的亲密度顺序
            int index = order[x][y];
            // 遍历顺序
            for (int i = 0; i < index; ++i) {
                int u = preferences[x][i];
                // 根据配对得到v
                int v = match[u];
                // 亲密度高则为unhappy 朋友
                if (order[u][x] < order[u][v]) {
                    ++unhappyCount;
                    break;
                }
            }
        }
        return unhappyCount;
    }
};
