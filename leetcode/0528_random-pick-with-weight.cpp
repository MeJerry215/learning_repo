#include "common.h"

class Solution {
public:
    int sum;
    vector<float> ranges;

    Solution(vector<int>& w) {
        sum = accumulate(w.begin(), w.end(), 0);
        ranges.push_back((float)w[0] / sum);
        for(int i = 1; i < w.size(); i++) {
            w[i] += w[i - 1];
            ranges.push_back(float(w[i]) / sum);
        }
        srand(42);
    }
    
    int pickIndex() {
        float val = (float)(rand() % sum)/ sum;
        return upper_bound(ranges.begin(), ranges.end(), val) - ranges.begin();
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(w);
 * int param_1 = obj->pickIndex();
 */