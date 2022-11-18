#include "common.h"


class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>> res(n, vector<int>(n, 0));
        int direction[5] = {0, 1, 0, -1, 0};
        int x = 0, y = 0;
        int s = n, t = n;
        int d = 0, step = s;
        for(int i = 0; i < n * n; i++) {
            res[x][y] = i + 1;
            step --;
            if (step == 0) {
                d = (d + 1) % 4;
                swap(s, t);
                s--;
                step = s;
            }
            x += direction[d];
            y += direction[d + 1];
        }
        return res;
    }
};