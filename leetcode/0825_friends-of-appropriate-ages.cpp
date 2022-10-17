#include "common.h"

/*
在社交媒体网站上有 n 个用户。给你一个整数数组 ages ，其中 ages[i] 是第 i 个用户的年龄。
如果下述任意一个条件为真，那么用户 x 将不会向用户 y（x != y）发送好友请求：
ages[y] <= 0.5 * ages[x] + 7
ages[y] > ages[x]
ages[y] > 100 && ages[x] < 100
否则，x 将会向 y 发送一条好友请求。
注意，如果 x 向 y 发送一条好友请求，y 不必也向 x 发送一条好友请求。另外，用户不会向自己发送好友请求。
返回在该社交媒体网站上产生的好友请求总数。

示例 1：
输入：ages = [16,16]
输出：2
解释：2 人互发好友请求。
示例 2：
输入：ages = [16,17,18]
输出：2
解释：产生的好友请求为 17 -> 16 ，18 -> 17 。
示例 3：
输入：ages = [20,30,100,110,120]
输出：3
解释：产生的好友请求为 110 -> 100 ，120 -> 110 ，120 -> 100 。

提示：
n == ages.length
1 <= n <= 2 * 104
1 <= ages[i] <= 120
来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/friends-of-appropriate-ages
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution
{
public:
    int numFriendRequests(vector<int>& ages) {
        vector<int> age(121, 0);//每个年龄有多少人
        for(auto a : ages) 
            age[a]++;
        vector<int> sum(121,0);
        for(int i = 1; i <= 120; i++)
        {
            sum[i] += sum[i-1]+age[i];// 前缀人数和
        }
        int ans = 0, l, r;
        for(int i = 15; i <= 120; i++)
        {
            l = i/2 + 7;
            r = i;
            ans += (sum[r-1]-sum[l])*age[i]+age[i]*(age[i]-1);
            //  年龄小于当前的，满足条件的人* 每人都可以发
            //  + 当前同龄里的人，互相发，排列 An2
        }
        return ans;
    }
};

/*
ages[y] <= 0.5 * ages[x] + 7
ages[y] > ages[x]
ages[y] > 100 && ages[x] < 100

满足以上3条件不是朋友，cond1 || cond2 || cond3 = !cond1 && !cond2 && !cond3

同时由于 cond2 满足的时候，必然导致cond3也满足，所以本题实际是两个条件。

所以 综合条件为 0.5 * ages[x] + 7 < ages[y] <= ages[x] 
如果要要上面的等式成立，求最小ages 即0.5 * ages[x] + 7 < ages[x]
ages[x] 最小为 15
*/
