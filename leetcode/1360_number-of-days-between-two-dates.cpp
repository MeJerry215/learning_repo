#include "common.h"

/*
请你编写一个程序来计算两个日期之间隔了多少天。
日期以字符串形式给出，格式为 YYYY-MM-DD，如示例所示。
 

示例 1：

输入：date1 = "2019-06-29", date2 = "2019-06-30"
输出：1
示例 2：

输入：date1 = "2020-01-15", date2 = "2019-12-31"
输出：15

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/number-of-days-between-two-dates
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    int daysBetweenDates(string date1, string date2) {
        return abs(get(date1) - get(date2));
    }

    bool isleap(int year) {
        return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    }

    int m_days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int get(string date) {
        int year, month, day;
        sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);

        int days = 0;
        for (int i = 1971; i < year; i++) days += 365 + isleap(i);

        for (int i = 1; i < month; i++) {
            if (i == 2) days += 28 + isleap(year);
            else days += m_days[i];
        }

        return days + day;
    }
};


void TestSolution() {
    Solution s;
    s.daysBetweenDates("2019-06-29", "2019-06-30");
}