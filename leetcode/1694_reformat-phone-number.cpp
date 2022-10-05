#include "common.h"


class Solution {
public:
    string reformatNumber(string number) {
        int i = 0;
        // eat all none digit.
        for(int j = 0; j < number.size(); j++) {
            if (isdigit(number[j])) {
                number[i++] = number[j];
            }
        }
        number.resize(i);
        string res = string(number.size() + (number.size() - 1) / 3, '+');
        int j = i = 0;
        while (i < number.size()) {
            res[j++] = number[i++];
            if (i % 3 == 0 && j < res.size() - 2) res[j++] = '-';
            if (number.size() % 3 == 1 && j == res.size() - 3) res[j++] = '-';
        }
        return res;
    }
};


void TestSolution() {
    Solution s;
    string number = "1-23-45 6";
    cout << s.reformatNumber(number) << endl;
    number = "123 4-567";
    cout << s.reformatNumber(number) << endl;
    number = "123 4-5678";
    cout << s.reformatNumber(number) << endl;
}