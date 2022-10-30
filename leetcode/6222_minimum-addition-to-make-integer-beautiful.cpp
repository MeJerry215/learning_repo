#include "common.h"


class Solution {
public:
    long long makeIntegerBeautiful(long long n, int target) {
        string num_str = to_string(n);
        int sum = 0;
        for(int i = 0; i < num_str.size(); i++) {
            sum += (num_str[i] - '0');
        }
        if (sum <= target) return 0;
        for(int i = num_str.size() - 1; i >= 0; i--) {
            sum -= (num_str[i] - '0');
            num_str[i] = '0';
            int k = i - 1;
            bool carry = true;
            while(k >= 0 && carry) {
                if (num_str[k] == '9') {
                    sum -= (num_str[k] - '0');
                    num_str[k] = '0';
                } else {
                    carry = false;
                    sum += 1;
                    num_str[k] += 1;
                }
                k--;
            }
            if (carry) {
                num_str = string("1") + string(num_str.size(), '0');
                break;
            } 
            if (sum <= target) break;
        }
        return atoll(num_str.c_str()) - n;
    }

    long long makeIntegerBeautiful(long long n, int target) {
        long long weight = 1LL;
        while (true){
            long long y = n + (weight - n % weight) % weight;

            int cur_sum = 0;
            long long tmp = y;
            while (tmp != 0LL){
                cur_sum += tmp % 10;
                tmp /= 10;
            }

            if (cur_sum <= target){
                return y - n;
            }

            weight *= 10;
        }
    }

};

void TestSolution() {
    Solution s;
    cout << s.makeIntegerBeautiful(712, 7) << endl;
}