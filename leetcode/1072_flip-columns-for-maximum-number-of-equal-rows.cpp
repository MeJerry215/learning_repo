#include "common.h"

/*
you are given an m x n binary matrix matrix.
You can choose any number of columns in the matrix and flip every cell in that column (i.e., Change the value of the cell from 0 to 1 or vice versa).
Return the maximum number of rows that have all values equal after some number of flips.


Example 1:
Input: matrix = [[0,1],[1,1]]
Output: 1
Explanation: After flipping no values, 1 row has all values equal.
Example 2:
Input: matrix = [[0,1],[1,0]]
Output: 2
Explanation: After flipping values in the first column, both rows have equal values.
Example 3:
Input: matrix = [[0,0,0],[0,0,1],[1,1,0]]
Output: 2
Explanation: After flipping values in the first two columns, the last two rows have equal values.
Constraints:

m == matrix.length
n == matrix[i].length
1 <= m, n <= 300
matrix[i][j] is either0 or 1.
*/


class Solution {
public:
    int maxEqualRowsAfterFlips(vector<vector<int>>& matrix) {
        int m = matrix.size();
        int n = matrix[0].size();
        unordered_map<bitset<300>,int> counts;
        for(int i = 0; i < m; i++) {
            bitset<300> bit_map(0);
            int val = matrix[i][0];
            for(int j = 0; j < n; j++) {
                bit_map.set(j, matrix[i][j] ^ val);
            }
            counts[bit_map] ++;
        }
        int ans = 0;
        for (auto iter = counts.begin(); iter != counts.end(); iter++) {
            ans = max(iter->second, ans);
        }
        return ans;
    }
};


void TestSolution() {
    Solution s;
    vector<vector<int>> matrix = {
        {0, 1},
        {1, 1}
    };
    cout << s.maxEqualRowsAfterFlips(matrix) << endl;
}