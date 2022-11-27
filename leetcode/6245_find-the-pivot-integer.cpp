#include "common.h"

class Solution {
public:
    int pivotInteger(int n) {
        int m = n * (n + 1) / 2;
        int x = sqrt(m);
        return x * x == m ? x: -1; 
    }
};

