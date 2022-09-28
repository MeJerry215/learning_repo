#include "common.h"

/*
你这个学期必须选修 numCourses 门课程，记为 0 到 numCourses - 1 。
在选修某些课程之前需要一些先修课程。 先修课程按数组 prerequisites 给出，其中 prerequisites[i] = [ai, bi] ，表示如果要学习课程 ai 则 必须 先学习课程  bi 。
例如，先修课程对 [0, 1] 表示：想要学习课程 0 ，你需要先完成课程 1 。
请你判断是否可能完成所有课程的学习？如果可以，返回 true ；否则，返回 false 。
示例 1：

输入：numCourses = 2, prerequisites = [[1,0]]
输出：true
解释：总共有 2 门课程。学习课程 1 之前，你需要完成课程 0 。这是可能的。
示例 2：
输入：numCourses = 2, prerequisites = [[1,0],[0,1]]
输出：false
解释：总共有 2 门课程。学习课程 1 之前，你需要先完成​课程 0 ；并且学习课程 0 之前，你还应先完成课程 1 。这是不可能的。
提示：

1 <= numCourses <= 105
0 <= prerequisites.length <= 5000
prerequisites[i].length == 2
0 <= ai, bi < numCourses
prerequisites[i] 中的所有课程对 互不相同

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/course-schedule
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/



class Solution
{
public:
    void print_degrees(unordered_map<int, unordered_set<int>> &degress)
    {
        auto iter = degress.begin();
        while (iter != degress.end())
        {
            cout << iter->first << ": ";
            auto iter2 = iter->second.begin();
            while (iter2 != iter->second.end())
            {
                cout << " " << *iter2;
                iter2++;
            }
            cout << endl;
            iter++;
        }
    }

    bool canFinish(int numCourses, vector<vector<int>> &prerequisites) {
        unordered_map<int, unordered_set<int>> indegrees;
        unordered_map<int, unordered_set<int>> outdegrees;
        for (const vector<int> &prerequisite : prerequisites) {
            indegrees[prerequisite[0]].insert(prerequisite[1]);
            outdegrees[prerequisite[1]].insert(prerequisite[0]);
        }

        vector<bool> visited(numCourses, false);
        while (true) {
            bool changed = false;
            for (int i = 0; i < numCourses; i++) {
                if (visited[i])
                    continue;
                cout << indegrees[i].size() << " " << i << endl;
                if (indegrees[i].size() == 0) {
                    visited[i] = true;
                    auto iter = outdegrees[i].begin();
                    while (iter != outdegrees[i].end()) {
                        if (indegrees[*iter].find(i) != indegrees[*iter].end()) {
                            changed = true;
                            indegrees[*iter].erase(i);
                        }
                        iter++;
                    }
                    continue;
                }
            }
            if (!changed)
                break;
        }

        auto iter = indegrees.begin();
        while (iter != indegrees.end()) {
            if (iter->second.size() != 0)
                return false;
            iter++;
        }

        return true;
    }
};