#include "common.h"


class Solution {
public:
    string tictactoe(vector<vector<int>>& moves) {
        /*
        统计落子数目，
        3行 A B落子状态 * 2, rowCount[3][2]
        同理 3列分别统计 colCount[3][2]
        然后两条对角线 * 2 分别统计 diagCount[2][2]。
        这里也看了评论区的一些解答，可以用+1 和 -1表示求和 表示此时的落子计数，A落子计数 +1, B 落子计数 -1, 可以优化掉数组空间
        */
        int rowCount[3][2] = {0};
        int colCount[3][2] = {0};
        int diagCount[2][2] = {0};
        for(int i = 0; i < moves.size(); i++) {
            vector<int>& move = moves[i];
            rowCount[move[0]][i % 2] ++;
            colCount[move[1]][i % 2] ++;
            if (move[0] == move[1]) {
                diagCount[0][i % 2] ++;
            }
            if (move[0] + move[1] == 2) {
                diagCount[1][i % 2] ++;
            }
            if (rowCount[move[0]][i % 2] == 3 || colCount[move[1]][i % 2] == 3|| diagCount[0][i % 2] == 3 || diagCount[1][i % 2] == 3) {
                return i % 2 ? "B" :  "A";
            }
        }
        return moves.size() == 9 ? "Draw" : "Pending";
    }
};