#include "common.h"

class Solution
{
public:
    vector<string> findRelativeRanks(vector<int> &score) {
        vector<int> indexs(score.size());
        iota(indexs.begin(), indexs.end(), 0);
        sort(indexs.begin(), indexs.end(), [&](const int lhs, const int rhs)
             { return score[lhs] > score[rhs]; });
        string rewards[3] = {"Gold Medal", "Silver Medal", "Bronze Medal"};
        vector<string> res(score.size());
        for (int i = 0; i < score.size(); i++) {
            if (i < 3) {
                res[indexs[i]] = rewards[i];
            } else {
                res[indexs[i]] = to_string(i + 1);
            }
        }
        return res;
    }
};