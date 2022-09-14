#include "common.h"

class Solution {
public:
    int maximumSwap(int num) {
        string num_str = to_string(num);
        string num_cpy = num_str;
        sort(num_cpy.begin(), num_cpy.end(), std::greater<char>());
        // std::cout << num_cpy << num_str <<std::endl;
        for(int i = 0; i < num_str.size(); i++) {
            if (num_str[i] != num_cpy[i]) {
                auto pos = num_str.rfind(num_cpy[i]);
                swap(num_str[pos], num_str[i]);
                break;
            }
        }
        return atoi(num_str.c_str());
    }
};

void TestSolution() {
    Solution s;
    std::cout << s.maximumSwap(9829223) << std::endl;
}