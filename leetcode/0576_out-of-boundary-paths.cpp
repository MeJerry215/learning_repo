#include "common.h"

class Solution
{
public:
    int directions[5] = {0, 1, 0, -1, 0};

    int mod = 1000000007;

    int findPaths(int m, int n, int maxMove, int startRow, int startColumn)
    {
        auto get_pos = [&](int x, int y) -> int {
            return (x * (n + 2) + y);
        };
        auto is_move_out = [&](int x, int y) -> bool {
            return  x == 0 || y == 0 || x == m + 1 || y == n + 1;
        };
        // auto print_mat_ = [&](int *mat) -> void {
        //     for(int i = 0; i < m + 2; i ++) {
        //         for(int j = 0; j < n + 2; j ++) {
        //             cout << mat[get_pos(i, j)] << "\t";
        //         }
        //         cout << endl;
        //     }
        // };
        int *pcur = new int[(m + 2) * (n + 2)];
        memset(pcur, 0, (m + 2) * (n + 2) * sizeof(int));
        int *pnext = new int[(m + 2) * (n + 2)];
        memset(pnext, 0, (m + 2) * (n + 2) * sizeof(int));
        int move = 0;
        pcur[get_pos(startRow + 1, startColumn + 1)] = 1;
        queue<pair<int, int>> indexs;
        indexs.emplace(startRow + 1, startColumn + 1);
        unordered_set<int> pos;
        while (move < maxMove)
        {
            move++;
            int k = indexs.size();
            // cout << "=========  move ========" << endl; 
            pos.clear();
            for (int i = 0; i < k; i++)
            {
                pair<int, int> &index = indexs.front();
                // cout << "try " << index.first << " " << index.second << endl;
                for (int j = 0; j < 4; j++)
                {
                    int x = index.first + directions[j];
                    int y = index.second + directions[j + 1];
                    pnext[get_pos(x, y)] += pcur[get_pos(index.first, index.second)];
                    pnext[get_pos(x, y)] %= mod;
                    if (!is_move_out(x, y) && !pos.count(x * 100 + y)) {
                        indexs.emplace(x, y);
                        pos.insert(x * 100 + y);
                    }
                }
                indexs.pop();

            }
            for(int j = 1; j < (m + 1); j ++) {
                pnext[get_pos(j, 0)] += pcur[get_pos(j, 0)];
                pnext[get_pos(j, n + 1)] += pcur[get_pos(j, n + 1)];
            }
            for(int j = 1; j < (n + 1); j++) {
                pnext[get_pos(0, j)] += pcur[get_pos(0, j)];
                pnext[get_pos(m + 1, j)] += pcur[get_pos(m + 1, j)];
            }
            // cout << "move " << move << endl;
            // cout << "cur is :" << endl;
            // print_mat_(pcur);
            // cout << "next is:" << endl;
            // print_mat_(pnext);
            memset(pcur, 0, (m + 2) * (n + 2) * sizeof(int));
            swap(pcur, pnext);
        }
        int res = 0;
        for(int j = 1; j < (m + 1); j ++) {
            res += pcur[get_pos(j, 0)];
            res %= mod;
            res += pcur[get_pos(j, n + 1)];
            res %= mod;
        }
        for(int j = 1; j < (n + 1); j++) {
            res += pcur[get_pos(0, j)];
            res %= mod;
            res += pcur[get_pos(m + 1, j)];
            res %=mod;
        }
        return res;
        // swap(cur, next);
    }
};


void TestSolution() {
    Solution s;
    cout << s.findPaths(8, 50, 23, 5, 26) << endl;
}


