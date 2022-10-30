#include "common.h"


class Solution {
public:
    int nextGreaterElement(int n) {
        string num_str = to_string(n);
        int i = num_str.size() - 2;
        while(i >= 0) {
            if (num_str[i] < num_str[i + 1]) {
                break;
            }
            i--;
        }
        if (i == -1) return -1;
        auto iter = upper_bound(num_str.rbegin(), num_str.rbegin() + num_str.size() - i - 1, num_str[i]);
        swap(*iter, num_str[i]);
        sort(num_str.begin() + i + 1, num_str.end());

        long long l = atoll(num_str.c_str());
        return l > INT_MAX ? -1 : l;
    }
};

void TestSolution() {
    Solution s;
    cout << s.nextGreaterElement(2147483486) << endl;
}
