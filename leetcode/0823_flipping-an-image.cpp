#include "common.h"

class Solution {
public:
    vector<vector<int>> flipAndInvertImage(vector<vector<int>>& image) {
        int n = image.size();
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < (n + 1) / 2; j++) {
                int k = n - j - 1;
                int p = !image[i][j];
                int q = !image[i][k];
                image[i][j] = q, image[i][k] = p;

            }
        }
        return image;
    }
};