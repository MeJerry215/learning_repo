#include "common.h"

class Solution
{
public:
    bool isValid(vector<vector<int>> &mat, int x, int y, int win_size, int threshold)
    {
        int sum = 0;
        for (int i = x; i < x + win_size; i++) {
            for (int j = y; j < y + win_size; j++) {
                sum += mat[i][j];
                if (sum > threshold)
                    return false;
            }
        }
        return true;
    }

    int maxSideLength(vector<vector<int>> &mat, int threshold)
    {
        int m = mat.size();
        int n = mat[0].size();
        // 用来加速搜索，如果在windows_size较小的时候, 窗口内找不到满足的，则更大的windows更加不会满足
        vector<vector<bool>> visited(m, vector<bool>(n, true));
        int win_size = 0;
        // win_size最大不会超过min(m, n)
        for (int i = 1; i <= min(m, n); i++) {
            bool isFind = false;
            for (int j = 0; j < m - i + 1; j++) {
                if (isFind)
                    break;
                for (int k = 0; k < n - i + 1; k++) {
                    if (!visited[j][k])
                        continue;
                    if (isValid(mat, j, k, i, threshold)) {
                        isFind = true;
                        win_size = i;
                        break;
                    } else {
                        visited[j][k] = false;
                    }
                }
            }
            // 如果整个循环内部都找不到一个，则更大的窗口也不会满足
            if (!isFind)
                break;
        }
        return win_size;
    }
};