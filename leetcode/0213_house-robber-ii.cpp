#include "common.h"
/*
小偷又发现了一个新的可行窃的地区。这个地区只有一个入口，我们称之为 root 。
除了 root 之外，每栋房子有且只有一个“父“房子与之相连。一番侦察之后，聪明的小偷意识到“这个地方的所有房屋的排列类似于一棵二叉树”。 如果 两个直接相连的房子在同一天晚上被打劫 ，房屋将自动报警。
给定二叉树的 root 。返回 在不触动警报的情况下 ，小偷能够盗取的最高金额 。

示例 1:
输入: root = [3,2,3,null,3,null,1]
输出: 7
解释: 小偷一晚能够盗取的最高金额 3 + 3 + 1 = 7
示例 2:


输入: root = [3,4,5,1,3,null,1]
输出: 9
解释: 小偷一晚能够盗取的最高金额 4 + 5 = 9
 
提示：

树的节点数在 [1, 104] 范围内
0 <= Node.val <= 104

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/house-robber-iii
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution
{
public:
    int rob(vector<int> &nums)
    {
        int n = nums.size();
        if (n <= 3)
            return *max_element(nums.begin(), nums.end());
        int p = nums[0], q = max(nums[0], nums[1]), r = nums[1], s = max(nums[1], nums[2]);
        int t = max(p + nums[2], q);
        p = q, q = t;
        for (int i = 3; i < n - 1; i++)
        {
            t = max(p + nums[i], q);
            p = q;
            q = t;
            t = max(r + nums[i], s);
            r = s;
            s = t;
        }
        t = max(r + nums[nums.size() - 1], s);
        cout << q << " " << t << endl;
        return max(t, q);
    }
};