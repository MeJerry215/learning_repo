#include "common.h"


class Solution {

public:
    int compare_overlap(vector<vector<int>>& img1, vector<vector<int>>& img2, int x_offset, int y_offset) {
        /* 这里只移动 img1, 所以img1的索引需要被重新计算，而img2 是不需要被重新计算的 */
        int overlap_num = 0;
        int n = img1.size();
        int y_iter = n + y_offset;

        if (x_offset <= 0) {
            int x_iter = n + x_offset;
            for(int i = 0; i < x_iter; i++) {
                for(int j = 0; j < y_iter; j++) {
                    if (img2[j][i] == 1 && img2[j][i] == img1[j - y_offset][i - x_offset]) {
                        overlap_num ++;
                    }
                }
            }
        } else {
            int x_iter = n;
            if (x_offset == n - 1 && y_offset == - (n - 1)) {
                cout << "x_iter " << x_iter << " " << y_iter << " " << x_offset << " " << y_offset <<   endl;
            } 
            for(int i = x_offset; i < x_iter; i++) {
                for(int j = 0; j < y_iter; j++) {
                    if (i == 4 && j == 0 && x_offset == 4 && y_offset == -4) {
                        cout << "img2[j][i] " << img2[j][i] << " img1[j - y_offset][i] " << img1[j - y_offset][i] << endl;
                        cout << i << " " << j << " " << j - y_offset << endl;
                    }
                    if (img2[j][i] == 1 && img2[j][i] == img1[j - y_offset][i]) {
                        overlap_num ++;
                    }
                }
            }
        }
        return overlap_num;
    }


    int largestOverlap(vector<vector<int>>& img1, vector<vector<int>>& img2) {
        int res = 0;
        int n = img1.size();
        /*
        img1 往下右偏移 等价于img2往上左偏移， 所以这里只考虑往上左偏移的情况，需要交换重复计算,
        同理 img1 往下左，等价img2往上右偏移。
         */
        for(int x_offset = - (n - 1); x_offset <= n - 1; x_offset++) {
            for(int y_offset = -(n - 1); y_offset <= 0; y_offset++) {
                res = max(res, compare_overlap(img1, img2, x_offset, y_offset));
                res = max(res, compare_overlap(img2, img1, x_offset, y_offset));
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

