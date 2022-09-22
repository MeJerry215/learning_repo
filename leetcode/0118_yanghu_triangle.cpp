#include "common.h"

class Solution {
 public:
  vector<vector<int>> generate(int numRows) {
    // 第i层 i + 1个数据
    vector<vector<int>> res(numRows, vector<int>());
    for (int i = 0; i < numRows; i++) {
      res[i].resize(i + 1);
      res[i][0] = 1;
      res[i][i] = 1;
      for (int j = 1; j < i; j++) {
        res[i][j] = res[i - 1][j] + res[i - 1][j - 1];
      }
    }
    return res;
  }
};