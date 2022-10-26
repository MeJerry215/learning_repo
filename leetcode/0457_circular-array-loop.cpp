#include "common.h"

class Solution
{
public:
    bool circularArrayLoop_v1(vector<int> &nums)
    {
        int n = nums.size();
        for (int i = 0; i < nums.size(); i++)
        {
            if (nums[i] == 0)
                continue;
            int loop_cnt = 0;
            bool is_positive_loop = nums[i] > 0;
            int cur = i;
            while (loop_cnt < n)
            {
                int next = ((cur + nums[cur]) % n + n) % n;
                if (next == cur || (nums[cur] > 0) != is_positive_loop)
                    break;
                cur = next;
                loop_cnt++;
            }
            if (loop_cnt == n)
                return true;
            cur = i;
            while (nums[cur] && (nums[cur] > 0) == is_positive_loop)
            {
                int next = ((cur + nums[cur]) % n + n) % n;
                nums[cur] = 0;
                cur = next;
            }
        }
        return false;
    }

    bool circularArrayLoop(vector<int> &nums)
    {
        int n = nums.size();
        auto next = [&](int cur)
        {
            return ((cur + nums[cur]) % n + n) % n; // 保证返回值在 [0,n) 中
        };

        for (int i = 0; i < n; i++)
        {
            if (!nums[i])
            {
                continue;
            }
            int slow = i, fast = next(i);
            // 判断非零且方向相同
            while (nums[slow] * nums[fast] > 0 && nums[slow] * nums[next(fast)] > 0)
            {
                if (slow == fast)
                {
                    if (slow != next(slow))
                    {
                        return true;
                    }
                    else
                    {
                        break;
                    }
                }
                slow = next(slow);
                fast = next(next(fast));
            }
            int add = i;
            while (nums[add] * nums[next(add)] > 0)
            {
                int tmp = add;
                add = next(add);
                nums[tmp] = 0;
            }
        }
        return false;
    }
};