#include "common.h"


class Solution {
public:
    int numberOfArrays(vector<int>& differences, int lower, int upper) {
        int low = differences[0] > 0 ? 0 : differences[0];
        int high = differences[0] > 0 ? differences[0] : 0;
        long long sum = differences[0];
        for(int i = 1; i < differences.size(); i++) {
            sum += differences[i];
            low = min((long long)low, sum);
            high = max((long long)high, sum);
        }
        int res = (upper - lower) - (high - low) + 1;
        return res > 0 ? res : 0;
    }
};