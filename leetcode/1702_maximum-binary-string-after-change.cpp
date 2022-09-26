#include "common.h"

/*
there has two converting rules, one is 00 to 10, the other is 10 to 01
00 to 10 always has postive impact, but 10 to 01 has side effect.
so the only use 10 to 01 is when has the pattern 0{1*n}0, by serveral transform,
the finally string convert to 10{1*{n-1}}
00 -> 10
10 -> 01
01*n0 -> 101*n
*/
class Solution {
public:
    void format(string& binary) {
        int n = binary.size();
        vector<int> zeroPos;
        for(int i = 0; i < n; i++) {
            if (binary[i] == '0') {
                zeroPos.push_back(i);
            }
        }
        /*
        idx     3 4 5 6 7 8 9
        val     0 1 1 1 1 0 1
        trans   1 0 1 1 1 1 1
        */
       for(int i = 0; i < zeroPos.size() - 1; i++) {
            int head = zeroPos[i];
            int tail = zeroPos[i + 1];
            if (tail - head == 1) {
                continue;
            } else {
                binary[head] = '1';
                binary[head + 1] = '0';
                binary[tail] = '1';
                zeroPos[i] = head + 1;
                zeroPos.erase(zeroPos.begin() + i + 1);
            }
        }
    }

    string maximumBinaryString(string binary) {
        int n = binary.size();
        // firstly replace all 00 to 10, it will make the string towards to larger num.
        format(binary);
        for(int i = 0; i < n - 1; i++) {
            if (binary[i] == '0' && binary[i + 1] == '0') {
                binary[i] = '1';
            }
        }
        // then find the pattern start with 0, intermidiate with serval 1, and finaly
        format(binary);
        return binary;
    }
};

void TestSolution() {
    Solution s;
    string binary = "1100";
    cout << s.maximumBinaryString(binary) << endl;
}


/*
Testcase
"01111001100000110010"
Answer
"11110111111111111111"
Expected Answer
"11111111110111111111"
*/