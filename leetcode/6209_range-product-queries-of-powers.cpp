#include "common.h"

/*
给你一个正整数 n ，你需要找到一个下标从 0 开始的数组 powers ，它包含 最少 数目的 2 的幂，且它们的和为 n 。powers 数组是 非递减 顺序的。根据前面描述，构造 powers 数组的方法是唯一的。
同时给你一个下标从 0 开始的二维整数数组 queries ，其中 queries[i] = [lefti, righti] ，其中 queries[i] 表示请你求出满足 lefti <= j <= righti 的所有 powers[j] 的乘积。
请你返回一个数组 answers ，长度与 queries 的长度相同，其中 answers[i]是第 i 个查询的答案。由于查询的结果可能非常大，请你将每个 answers[i] 都对 109 + 7 取余 。

示例 1：
输入：n = 15, queries = [[0,1],[2,2],[0,3]]
输出：[2,4,64]
解释：
对于 n = 15 ，得到 powers = [1,2,4,8] 。没法得到元素数目更少的数组。
第 1 个查询的答案：powers[0] * powers[1] = 1 * 2 = 2 。
第 2 个查询的答案：powers[2] = 4 。
第 3 个查询的答案：powers[0] * powers[1] * powers[2] * powers[3] = 1 * 2 * 4 * 8 = 64 。
每个答案对 109 + 7 得到的结果都相同，所以返回 [2,4,64] 。
示例 2：
输入：n = 2, queries = [[0,0]]
输出：[2]
解释：
对于 n = 2, powers = [2] 。
唯一一个查询的答案是 powers[0] = 2 。答案对 109 + 7 取余后结果相同，所以返回 [2] 。

提示：

1 <= n <= 109
1 <= queries.length <= 105
0 <= starti <= endi < powers.length
*/

class Solution {
public:
    constexpr static int mod = 1000000007;


    vector<int> productQueries(int n, vector<vector<int>>& queries) {
        /*
        这道题难得是读懂题目。。。
        首先是 n = accumulate(powers.begin(), powers.end(), 0). 首先是找到一个powers数组
        对于powers中的元素每一个都是2的幂次方。对于给的示例我不是很喜欢。而且powers数组必须是最短的(最少数目的2的幂)
        对于 n = 21 产生的幂次方为 [1, 4, 16]
        15 = 1111
        21 = 10101
        */
        /*
            构造幂次方数组 powers
        */
        vector<int> powers;
        int base = 1;
        while(n) {
            if (n & 0x1) powers.push_back(base);
            base *= 2;
            n >>=1;
        }
        print_vec(powers);
        /*
        获得power数组之后就是要计算range之内的乘积，唯一要注意的一点就是越界了进行mod 乘
        如果 i < j < k   mod 取 13
        i * j 的结果将大于 13 则本步骤计算的答案为 (i * j) % 13
        如果再乘上一个 k，问此时如何计算。
        以下公式是否相等？
        (i * j * k) % 13 =? ((i * j) % 13 ) * k % 13
        i * j % 13 = i * j - i * j / 13 * 13
        而 i * j / 13 * 13 必然为13的倍数关系, 计为 13 * z
        (i * j) % 13 ) * k = i * j * k - 13 * z * k
        所以 以上公式是等价的。
        */
        vector<int> res;
        for(auto& query: queries) {
            int low = query[0];
            int total = 1;
            while(low <= query[1]) {
                total *= powers[low++];
                total %= mod;
            }
            res.push_back(total);
        }
        return res;
    }
};


void TestSolution() {
    Solution s;
    vector<vector<int>> queries = {};
    s.productQueries(15, queries);
}