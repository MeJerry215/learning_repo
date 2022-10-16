#include "common.h"

/*
有一棵 n 个节点的无向树，节点编号为 0 到 n - 1 。

给你一个长度为 n 下标从 0 开始的整数数组 nums ，其中 nums[i] 表示第 i 个节点的值。同时给你一个长度为 n - 1 的二维整数数组 edges ，其中 edges[i] = [ai, bi] 表示节点 ai 与 bi 之间有一条边。

你可以 删除 一些边，将这棵树分成几个连通块。一个连通块的 价值 定义为这个连通块中 所有 节点 i 对应的 nums[i] 之和。

你需要删除一些边，删除后得到的各个连通块的价值都相等。请返回你可以删除的边数 最多 为多少。

 

示例 1：



输入：nums = [6,2,2,2,6], edges = [[0,1],[1,2],[1,3],[3,4]]
输出：2
解释：上图展示了我们可以删除边 [0,1] 和 [3,4] 。得到的连通块为 [0] ，[1,2,3] 和 [4] 。每个连通块的价值都为 6 。可以证明没有别的更好的删除方案存在了，所以答案为 2 。
示例 2：

输入：nums = [2], edges = []
输出：0
解释：没有任何边可以删除。
 

提示：

1 <= n <= 2 * 104
nums.length == n
1 <= nums[i] <= 50
edges.length == n - 1
edges[i].length == 2
0 <= edges[i][0], edges[i][1] <= n -

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/create-components-with-same-value
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

/*
对于无向树，不成环。

对于连通性 计算总sum 和 分块连通区域数量 k, 则每个连通区域的和应该为 total/k，如果无法满足则结果有问题。
*/

class Solution {
public:
    int componentValue(vector<int>& nums, vector<vector<int>>& edges) {
        int n = nums.size(), target = 0;
        vector<vector<int>> g(n);
        for (auto &edge : edges) {
            g[edge[0]].push_back(edge[1]);
            g[edge[1]].push_back(edge[0]);
        }
        function<int(int, int)> dfs = [&](int x, int fa) {
            int sum = nums[x];
            for (int y : g[x]) {
                if (y != fa) {
                    int res = dfs(y, x);
                    if (res < 0) return -1;
                    sum += res;
                }
            }
            if (sum > target) return -1;
            return sum < target ? sum : 0;
        };
        int total = accumulate(nums.begin(), nums.end(), 0);
        int mx = *max_element(nums.begin(), nums.end());
        for (int i = min(n, total / mx);; --i) {
            if (total % i == 0) {
                target = total / i;
                if (dfs(0, -1) == 0) return i - 1;
            }
        }
        return -1;
    }
};