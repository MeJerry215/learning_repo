#include "common.h"


class Solution {
public:
    int magicalString(int n) {
    if (n < 2) return n;
    string magic_str = "122";
    int cnt = 1;
    for (int cur = 2; cur < n; cur++) {
      if (magic_str[cur] == '1') {
        cnt++;
        magic_str.push_back(magic_str.back() ^ 3);
      } else {
        magic_str += string(2, magic_str.back() ^ 3);
      }
    }
    return cnt;
  }
};