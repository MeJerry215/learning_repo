#include "common.h"


class Solution {
public:

    int gcd(int m,int n)
    {
        int z = n;
        while (m % n)
        {
            z = m % n;
            m = n;
            n = z;
        }
        return z;
    }


    int subarrayGCD_v1(vector<int>& nums, int k) {
        int n = nums.size();
        int res = 0;
        for(int i = 0; i < n; i++) {
            if (nums[i] % k) continue;
            int g = 0;
            for (int j = i; j < n; j++) {
                g = gcd(g, nums[j]);
                if (g == k) res ++;
            }
        }
        return res;
    }

    int subarrayGCD(vector<int>& nums, int k) {
        int n = nums.size();
        int res = 0;
        vector<pair<int, int>> gcds;
        int invaid = -1;
        for(int i = 0; i < nums.size(); i++) {
            if (nums[i] % k) {
                gcds.clear();
                invaid = i;
                continue;
            }
            gcds.push_back({nums[i], i});
            int j = 0;
            for (pair<int,int>& gc: gcds) {
                gc.first= gcd(gc.first, nums[i]);
                if (gc.first != gcds[j].first) {
                    j ++;
                    gcds[j].first = gc.first;
                }
                gcds[j].second = gc.second;

            }
            gcds.resize(j + 1);
            if (gcds[0].first == k) {
                res += gcds[0].second - invaid;
            }
        }
        return res;
    }
};