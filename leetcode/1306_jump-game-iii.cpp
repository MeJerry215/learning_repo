#include "common.h"

class Solution
{
public:
    bool canReach(vector<int> &arr, int start)
    {
        /*
        记忆化搜索, 对于已经确定无法达到的节点，无论是往前还是往后都无法到达刷为INT_MAX,
        在当前遍历过程中如果为已经遍历的节点记为INT_MIN
        */
        function<bool(int)> visit = [&](int pos)
        {
            if (pos >= arr.size() || pos < 0 || arr[pos] == INT_MAX)
                return false;
            if (arr[pos] == 0)
                return true;
            int val = arr[pos];
            arr[pos] = INT_MAX;
            if (visit(pos + val) || visit(pos - val))
                return true;
            return false;
        };
        return visit(start);
    }
};