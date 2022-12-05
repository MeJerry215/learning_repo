#include "common.h"

/*
给你一个正整数数组 skill ，数组长度为 偶数 n ，其中 skill[i] 表示第 i 个玩家的技能点。将所有玩家分成 n / 2 个 2 人团队，使每一个团队的技能点之和 相等 。
团队的 化学反应 等于团队中玩家的技能点 乘积 。
返回所有团队的 化学反应 之和，如果无法使每个团队的技能点之和相等，则返回 -1 。

 

示例 1：

输入：skill = [3,2,5,1,3,4]
输出：22
解释：
将玩家分成 3 个团队 (1, 5), (2, 4), (3, 3) ，每个团队的技能点之和都是 6 。
所有团队的化学反应之和是 1 * 5 + 2 * 4 + 3 * 3 = 5 + 8 + 9 = 22 。
示例 2：

输入：skill = [3,4]
输出：12
解释：
两个玩家形成一个团队，技能点之和是 7 。
团队的化学反应是 3 * 4 = 12 。
示例 3：

输入：skill = [1,1,2,3]
输出：-1
解释：
无法将玩家分成每个团队技能点都相等的若干个 2 人团队。
 

提示：

2 <= skill.length <= 105
skill.length 是偶数
1 <= skill[i] <= 1000

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/divide-players-into-teams-of-equal-skill
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution {
public:
    long long dividePlayers_v1(vector<int>& skill) {
        int n = skill.size();
        unordered_map<int, int> skill_cnt;
        int sum = 0;
        for(auto s: skill) {
            skill_cnt[s]++;
            sum += s;
        }
        if (sum % (n / 2)) return -1; 
        int target = sum / (n / 2);
        long long res = 0;
        unordered_set<int> visited;
        for(auto iter : skill_cnt) {
            if (visited.count(iter.first)) continue;
            if (iter.second != skill_cnt[target - iter.first]) return -1;
            bool same = iter.first == target - iter.first;
            if (same && iter.second % 2) return -1; 
            res += (long long)(iter.first * (target - iter.first)) * (same ? iter.second / 2: iter.second);
            visited.insert(target - iter.first);
        }
        return res;
    }

    long long dividePlayers(vector<int>& skill) {
        
    }
};
