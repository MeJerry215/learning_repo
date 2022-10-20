#include "common.h"

class Solution {
public:
    int countCollisions_v1(string directions) {
        int res = 0;
        for(int i = 1; i < directions.size(); i++) {
            // RL
            if (directions[i - 1] == 'R' && directions[i] == 'L') {
                res += 2;
                directions[i - 1] = directions[i] = 'S';
            }
        }
        for(int i = 0; i < directions.size(); i++) {
            if (directions[i] == 'S') {
                int j = i - 1;
                while(j >= 0 && directions[j] == 'R') {
                    res ++;
                    directions[j] = 'S';
                }
                j = i + 1;
                while(j < directions.size() && directions[j] == 'L') {
                    res ++;
                    directions[j] == 'S';
                }
            }
        }
        return res;
    }

    int countCollisions(string s) {
        int l = 0, r = s.size() - 1;
        while(l <= r && s[l] == 'L') ++l;
        while(l <= r && s[r] == 'R') --r;
        int res = 0; 
        for(int i = l; i <= r; ++i) if(s[i] == 'L' || s[i] == 'R') ++res;
        return res;
    }

};
