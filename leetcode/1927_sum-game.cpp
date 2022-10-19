#include "common.h"

class Solution {
public:
    bool sumGame(string num)
    {
        int n = num.size();
        int cnt = 0;
        int mid = n / 2;
        int sum = 0;
        for (int i = 0; i < n; i++) {
            if (i < mid) {
                if (num[i] == '?')
                    cnt++;
                else
                    sum += (num[i] - '0');
            } else {
                if (num[i] == '?')
                    cnt--;
                else
                    sum -= (num[i] - '0');
            }
        }
        if (cnt * sum > 0)
            return true;
        cnt = abs(cnt);
        sum = abs(sum);
        if ((cnt + 1) / 2 * 9 > sum)
            return true;
        if (cnt / 2 * 9 == sum)
            return false;
        return true;
    }
};