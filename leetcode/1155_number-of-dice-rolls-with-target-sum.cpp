#include "common.h"


/*
这里有 n 个一样的骰子，每个骰子上都有 k 个面，分别标号为 1 到 k 。
给定三个整数 n ,  k 和 target ，返回可能的方式(从总共 kn 种方式中)滚动骰子的数量，使正面朝上的数字之和等于 target 。
答案可能很大，你需要对 109 + 7 取模 。
 

示例 1：

输入：n = 1, k = 6, target = 3
输出：1
解释：你扔一个有6张脸的骰子。
得到3的和只有一种方法。
示例 2：

输入：n = 2, k = 6, target = 7
输出：6
解释：你扔两个骰子，每个骰子有6个面。
得到7的和有6种方法1+6 2+5 3+4 4+3 5+2 6+1。
示例 3：

输入：n = 30, k = 30, target = 500
输出：222616187
解释：返回的结果必须是对 109 + 7 取模。
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/number-of-dice-rolls-with-target-sum
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution {
public:
    int numRollsToTarget(int n, int k, int target) {
        /* 最基础的想法是 dfs, 然而当我们看到其可能解空间 n = 30, k = 30, 时间复杂度30 ^ 30, 这是个天文数字，所以如果这样算就不太行了。
        而如果要减少这样的时间复杂度，可以靠dp, 
        */
        unordered_map<int, int> sum_counts;
        sum_counts.insert({0, 1});
        for(int i = 0; i < n; i++) {
            unordered_map<int, int> counts_tmp;
            for(auto iter: sum_counts) {
                for(int j = 1; j <= k; j++) {
                    if (iter.first + j <= target) {
                        counts_tmp[iter.first + j] += iter.second;
                        counts_tmp[iter.first + j] =  counts_tmp[iter.first + j] % (1000000000 + 7 );
                    } else {
                        break;
                    }
                }
            }
            sum_counts = move(counts_tmp);
        }
        for(auto iter: sum_counts) {
            if (iter.first == target) {
                return iter.second;
            }
        }
        return 0;
    }
};

void TestSolution() {
    Solution s;
    int n = 30;
    int k = 30;
    int target = 500;
    cout << s.numRollsToTarget(n, k, target) << endl;
}