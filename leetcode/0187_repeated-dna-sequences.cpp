#include "common.h"


class Solution {
public:
    unordered_map<int, char> dns_remap = {
            {0, 'A'}, {1, 'C'}, {2, 'G'}, {3, 'T'}
    };

    string decode(int encode) {
        string decode_str = "";
        for(int i = 0; i < 10; i++) {
            decode_str = dns_remap[encode & 0x3] + decode_str;
            encode = encode >> 2;
        }
        return decode_str;
    }

    vector<string> findRepeatedDnaSequences(string s) {
        unordered_map<int, int> sequence_count;
        unordered_map<char, int> dns_mapping = {
            {'A', 0}, {'C', 1}, {'G', 2}, {'T', 3}
        };
        vector<string> res;
        int mask = 0xFFFFF;
        int encode = 0;
        for(int i = 0; i < 10; i++) {
            encode = encode << 2;
            encode += dns_mapping[s[i]];
        }
        sequence_count[encode] ++;
        for(int i = 10; i < s.size(); i++) {
            encode = encode << 2;
            encode = encode & mask;
            encode += dns_mapping[s[i]];
            sequence_count[encode] ++;
            if (sequence_count[encode] == 2) {
                res.push_back(decode(encode));
            }
        }
        return res;
    }
};

void TestSolution() {
    Solution s;
    string str = "CCCCCCCCCCC";
    auto res = s.findRepeatedDnaSequences(str);
    print_vec(res);
}