#include "common.h"

class Solution
{
public:
    bool isValidSudoku(vector<vector<char>> &board)
    {
        constexpr int NUM = 9;
        bitset<NUM> row_cnt[NUM], col_cnt[NUM], box_cnt[NUM];
        for (int i = 0; i < NUM; i++)
        {
            for (int j = 0; j < NUM; j++)
            {
                if (board[i][j] == '.')
                    continue;
                int shift = board[i][j] - '1';
                if (row_cnt[i].test(shift) || col_cnt[j].test(shift) || box_cnt[i / 3 * 3 + j / 3].test(shift)) {
                    return false;
                }
                row_cnt[i].set(shift);
                col_cnt[j].set(shift);
                box_cnt[i / 3 * 3 + j / 3].set(shift);
            }
        }
        return true;
    }
};