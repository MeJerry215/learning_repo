#include "common.h"


/*
查分的性质, 对于一个区间操作
 1 2 3 4 5 6   1, 3, 2 对于查分数组产生的影响为 1 4 5 6 5 6
 1 1 1 1 1 1                                  1 3 1 1 -1 1
 产生的变化为 diff[l] + 2 diff[r + 1] - 2
*/
class Solution {
public:
    vector<int> getModifiedArray(int length, vector<vector<int>>& updates) {
        vector<int> res(length, 0);
        for(auto& update: updates) {
            res[update[0]] += update[2];
            if (update[1] + 1 < length) res[update[1] + 1] -= update[2];
        }
        for(int i = 1; i < length; i++) {
            res[i] += res[i - 1];
        }
        return res;
    }
};