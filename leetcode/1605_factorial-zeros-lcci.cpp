#include "common.h"

/*
设计一个算法，算出 n 阶乘有多少个尾随零。
示例 1:
输入: 3
输出: 0
解释: 3! = 6, 尾数中没有零。
示例 2:
输入: 5
输出: 1
解释: 5! = 120, 尾数中有 1 个零.
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/factorial-zeros-lcci
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

你算法的时间复杂度应为 O(log n) 
*/



/*
第一步： 先看题目，理解问题类型，和求解什么
求阶乘结果的0的个数，最通俗的解法算法复杂度在O(n)

第二步: 提示中的参数限制信息
提示算法复杂度应该为O(log n), 这必然得通过二分搜索法解决


第三步: 构造思路，明确考察类型
暂时没啥思路 只能通过找规律，O(log n) 
20

第四步: 用例失败分析
NA

第五步: AC 分析
执行用时：
236 ms, 在所有 C++ 提交中击败了 78.49%的用户
内存消耗：
64.7 MB, 在所有 C++ 提交中击败了52.33%的用户
通过测试用例：
54 / 54
从结果最后运行耗时来看，这个一定是构造了一个特别大的测试用力，所以如果用了其他办法会执行超时
*/

class Solution {
public:
    int trailingZeroes(int n) {
        int count = 0;
        while(n >= 5){
            n /= 5;
            count += n;
        }
        return count;
    }
};


void TestSolution() {
    Solution s;
}