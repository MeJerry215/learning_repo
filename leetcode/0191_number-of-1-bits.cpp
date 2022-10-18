#include "common.h"

class Solution {
public:
    int hammingWeight(uint32_t n) {
      int res = 0;
      while(n) {
        if (n & 0x1) res++;
        n = n / 2;
      }
      return res;
  }
};