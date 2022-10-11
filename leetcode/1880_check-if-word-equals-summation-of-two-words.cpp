#include "common.h"

class Solution {
public:
    bool isSumEqual(string firstWord, string secondWord, string targetWord) {
        auto sum = [](const string& s){return accumulate(s.begin(), s.end(), 0, [](const int& sum, const int& next){return sum * 10 + next - 'a';});};
        return sum(firstWord) + sum(secondWord) == sum(targetWord);
    }
};