#include "common.h"


/*
有些数的素因子只有 3，5，7，请设计一个算法找出第 k 个数。注意，不是必须有这些素因子，而是必须不包含其他的素因子。例如，前几个数按顺序应该是 1，3，5，7，9，15，21。
示例 1:
输入: k = 5
输出: 9

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/get-kth-magic-number-lcci
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
/*
    1      1
*/

class Solution {
public:
    int getKthMagicNumber(int k) {
        vector<int> magic_nums(k + 1, 1);
        int x3 = 1, x5 = 1, x7 = 1;
        for(int i = 2; i <= k; i++) {
            int x3_val = magic_nums[x3] * 3;
            int x5_val = magic_nums[x5] * 5;
            int x7_val = magic_nums[x7] * 7;
            int min_val = min(x3_val, min(x5_val, x7_val));
            if (min_val == x3_val) {
                x3++;
            }
            if (min_val == x5_val) {
                x5++;
            }
            if (min_val == x7_val) {
                x7++;
            }
            magic_nums[i] = min_val;
        }
        return magic_nums[k];
    }
};

void TestSolution() {
    Solution s;
    cout << s.getKthMagicNumber(2) << endl;
    cout << s.getKthMagicNumber(3) << endl;
    cout << s.getKthMagicNumber(4) << endl;
    cout << s.getKthMagicNumber(5) << endl;
    cout << s.getKthMagicNumber(6) << endl;
    cout << s.getKthMagicNumber(7) << endl;
}