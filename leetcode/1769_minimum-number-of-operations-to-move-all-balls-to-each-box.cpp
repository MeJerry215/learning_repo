#include "common.h"

class Solution
{
public:
    vector<int> minOperations(string boxes)
    {
        int l = 0, r = 0;
        int sum = 0;
        for (int i = 1; i < boxes.size(); i++) {
            if (boxes[i] == '1') {
                r++;
                sum += i * (boxes[i] - '0');
            }
        }
        vector<int> res;
        res.push_back(sum);
        for (int i = 1; i < boxes.size(); i++) {
            sum -= r;
            if (boxes[i] == '1') {
                r--;
            }
            if (boxes[i - 1] == '1') {
                l++;
            }
            sum += l;
            res.push_back(sum);
        }
        return res;
    }
};