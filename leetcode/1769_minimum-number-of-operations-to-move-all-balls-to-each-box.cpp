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

    vector<int> minOperations(string boxes) {
        vector<int> res(boxes.size(), 0);
        int ball_cnt = boxes[0] == '1';
        int k = 0;
        for(int i = 1; i < boxes.size(); i++) {
            res[i] = k + ball_cnt;
            k = res[i];
            ball_cnt += (boxes[i] == '1');
        }
        ball_cnt = boxes[boxes.size() - 1] == '1';
        k = 0;

        for(int i = boxes.size() - 2; i >=0 ;i -- ) {
            res[i] += (k + ball_cnt);
            k = res[i];
            ball_cnt += (boxes[i] == '1');
        }
        return res;
    }
};