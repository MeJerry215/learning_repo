#include "common.h"

/*
可以用字符串表示一个学生的出勤记录，其中的每个字符用来标记当天的出勤情况（缺勤、迟到、到场）。记录中只含下面三种字符：
'A'：Absent，缺勤
'L'：Late，迟到
'P'：Present，到场
如果学生能够 同时 满足下面两个条件，则可以获得出勤奖励：
按 总出勤 计，学生缺勤（'A'）严格 少于两天。
学生 不会 存在 连续 3 天或 连续 3 天以上的迟到（'L'）记录。
给你一个整数 n ，表示出勤记录的长度（次数）。请你返回记录长度为 n 时，可能获得出勤奖励的记录情况 数量 。答案可能很大，所以返回对 109 + 7 取余 的结果。

示例 1：
输入：n = 2
输出：8
解释：
有 8 种长度为 2 的记录将被视为可奖励：
"PP" , "AP", "PA", "LP", "PL", "AL", "LA", "LL"
只有"AA"不会被视为可奖励，因为缺勤次数为 2 次（需要少于 2 次）。
示例 2：
输入：n = 1
输出：3
示例 3：
输入：n = 10101
输出：183236316

提示：
1 <= n <= 105

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/student-attendance-record-ii
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

/*
    1  2  3  4
P00 1  2  4  7  "PPP" "LPP" "PLP" "LLP"  = P00 + L10 + L20
L10 1  1  2  4  "PPL" "LPL"              = P00
L20 0  1  1  2  "PLL"                    = L10
A01 1  2  4  7  "PPA" "LPA" "PLA" "LLA"  = P00 + L10 + L20
P01 0  1  4  12 "PAP" "LAP" "APP" "ALP"  = A01 + P01 + L11 + L21
L11 0  1  3  8  "PAL" "APL" "LAL"        = A01 + P01
L21 0  0  1  3  "ALL"                    = L11
*/

class Solution
{
public:
    int checkRecord(int n)
    {
        constexpr int mod = 1e9 + 7;
        vector<long long> yesterday = {1, 1, 0, 1, 0, 0, 0};
        vector<long long> today = {0, 0, 0, 0, 0, 0, 0};
        int i = 1;
        long long res = 0;
        while (i < n){
            today[0] = (yesterday[0] + yesterday[1] + yesterday[2]) % mod;
            today[1] = yesterday[0];
            today[2] = yesterday[1];
            today[3] = today[0];
            today[5] = (yesterday[3] + yesterday[4]) % mod;
            today[4] = (today[5] + yesterday[5] + yesterday[6]) % mod;
            today[6] = yesterday[5];
            i++;
            swap(today, yesterday);
        }
        res = accumulate(yesterday.begin(), yesterday.end(), 0LL);
        return res % mod;
    }

    int checkRecord(int n)
    {
        constexpr int mod = 1e9 + 7;
        vector<int> yesterday = {1, 1, 0, 1, 0, 0, 0};
        vector<int> today = {0, 0, 0, 0, 0, 0, 0};
        int i = 1;
        long long res = 0;
        while (i < n){
            today[0] = (yesterday[0] + yesterday[1] + yesterday[2]) % mod;
            today[1] = yesterday[0];
            today[2] = yesterday[1];
            today[3] = today[0];
            today[5] = (yesterday[3] + yesterday[4]) % mod;
            today[4] = (today[5] + yesterday[5] + yesterday[6]) % mod;
            today[6] = yesterday[5];
            i++;
            swap(today, yesterday);
        }
        for(int i = 0; i < yesterday.size(); i++) {
            res = (res + yesterday[i]) % mod;
        }
        return res;
    }
};