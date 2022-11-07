#include "common.h"

class Solution
{
public:
    vector<int> twoSum(vector<int> &numbers, int target)
    {
        int i = 0, j = numbers.size() - 1;
        while(i < j) {
            int sum = numbers[i] + numbers[j];
            if (sum == target) return {i, j};
            else if (sum < target) {
                i ++;
            } else {
                j --;
            }
        }
        return {-1, -1};
    }
};