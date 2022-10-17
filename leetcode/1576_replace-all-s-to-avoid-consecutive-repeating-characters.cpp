#include "common.h"


class Solution {
public:
    string modifyString(string s) {
    for(int i = 0; i < s.size(); i++) {
        if (s[i] == '?') {
          char l = (i - 1) >= 0 ? s[i - 1] : ' ';
          char r = (i + 1) < s.size() ? s[i + 1] : ' ';
          char c = 'a';
          for(int j = 0; j < 3; j++) {
            if (l == c || r == c) c++;
          }
          s[i] = c;
        }
    }
    return s;
  }
};