#include "common.h"

class Solution
{
public:
    bool searchMatrix(vector<vector<int>> &matrix, int target)
    {
        int m = matrix.size();
        int n = matrix[0].size();
        int low = 0, high = m * n - 1;
        while (low <= high)
        {
            int mid = (low + high) / 2;
            int x = mid / n;
            int y = mid % n;
            if (matrix[x][y] == target)
                return true;
            if (matrix[x][y] < target)
            {
                low = mid + 1;
            }
            else
            {
                high = mid - 1;
            }
        }
        return false;
    }
};