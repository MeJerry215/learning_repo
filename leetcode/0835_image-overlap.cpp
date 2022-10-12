#include "common.h"


class Solution
{

public:
    int compare_overlap(vector<vector<int>> &img1, vector<vector<int>> &img2, int x_offset, int y_offset)
    {
        int res = 0;
        int n = img1.size();
        int x_low = max(0, x_offset), x_high = min(n, n + x_offset);
        int y_low = max(0, y_offset), y_high = min(n, n + y_offset);
        for (int i = y_low; i < y_high; i++)
        {
            for (int j = x_low; j < x_high; j++)
            {
                if (img2[i][j] == 1 && img2[i][j] == img1[i - y_offset][j - x_offset])
                {
                    res++;
                }
            }
        }
        return res;
    }

    int largestOverlap(vector<vector<int>> &img1, vector<vector<int>> &img2)
    {
        int n = img1.size();
        int res = 0;
        /*
        总是移动nums1, nums2固定不动。使用x_offset, y_offset表示nums1的偏移。
        x_offset = -1, y_offset = -1 表示nums1 向左上各偏移1个单位。
        而nums1可以偏移的的范围至少要保证一个相交点存在所以偏移范围为1 - n < x_offset, y_offset < n - 1
         */
        for (int y_offset = 1 - n; y_offset <= n - 1; y_offset++)
        {
            for (int x_offset = 1 - n; x_offset <= n - 1; x_offset++)
            {
                res = max(res, compare_overlap(img1, img2, x_offset, y_offset));
            }
        }
        return res;
    }
};


void TestSolution() {
    Solution s;
    vector<vector<int>> img1 = {
        {0,0,0,0,1},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}
    };
    vector<vector<int>> img2 = {
        {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{1,0,0,0,0}
    };
    cout << s.largestOverlap(img1, img2) << endl;
};

