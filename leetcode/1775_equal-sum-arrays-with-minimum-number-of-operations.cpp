#include "common.h"

/*
给你两个长度可能不等的整数数组 nums1 和 nums2 。两个数组中的所有值都在 1 到 6 之间（包含 1 和 6）。
每次操作中，你可以选择 任意 数组中的任意一个整数，将它变成 1 到 6 之间 任意 的值（包含 1 和 6）。
请你返回使 nums1 中所有数的和与 nums2 中所有数的和相等的最少操作次数。如果无法使两个数组的和相等，请返回 -1 。
示例 1：
输入：nums1 = [1,2,3,4,5,6], nums2 = [1,1,2,2,2,2]
输出：3
解释：你可以通过 3 次操作使 nums1 中所有数的和与 nums2 中所有数的和相等。以下数组下标都从 0 开始。
- 将 nums2[0] 变为 6 。 nums1 = [1,2,3,4,5,6], nums2 = [6,1,2,2,2,2] 。
- 将 nums1[5] 变为 1 。 nums1 = [1,2,3,4,5,1], nums2 = [6,1,2,2,2,2] 。
- 将 nums1[2] 变为 2 。 nums1 = [1,2,2,4,5,1], nums2 = [6,1,2,2,2,2] 。
示例 2：
输入：nums1 = [1,1,1,1,1,1,1], nums2 = [6]
输出：-1
解释：没有办法减少 nums1 的和或者增加 nums2 的和使二者相等。
示例 3：
输入：nums1 = [6,6], nums2 = [1]
输出：3
解释：你可以通过 3 次操作使 nums1 中所有数的和与 nums2 中所有数的和相等。以下数组下标都从 0 开始。
- 将 nums1[0] 变为 2 。 nums1 = [2,6], nums2 = [1] 。
- 将 nums1[1] 变为 2 。 nums1 = [2,2], nums2 = [1] 。
- 将 nums2[0] 变为 4 。 nums1 = [2,2], nums2 = [4] 。

提示：

1 <= nums1.length, nums2.length <= 105
1 <= nums1[i], nums2[i] <= 6

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/equal-sum-arrays-with-minimum-number-of-operations
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

class Solution
{
public:
    int minOperations_v1(vector<int> &nums1, vector<int> &nums2)
    {
        int s1 = accumulate(nums1.begin(), nums1.end(), 0);
        int s2 = accumulate(nums2.begin(), nums2.end(), 0);
        if (s1 == s2)
            return 0;
        if (s1 < s2)
        {
            // make suren nums1 alwalys greater one
            swap(s1, s2);
            swap(nums1, nums2);
        }
        // sort nums1 in the order decending, nums2 asendding
        sort(nums1.begin(), nums1.end(), greater<int>());
        sort(nums2.begin(), nums2.end());
        int res = 0, diff = s1 - s2;
        int i = 0, j = 0;
        while (i < nums1.size() || j < nums2.size())
        {
            if (i < nums1.size() && j < nums2.size())
            {
                if (nums1[i] == 1 && nums2[j] == 6)
                    return -1;
                if (nums1[i] + nums2[j] >= 7)
                {
                    diff -= (nums1[i++] - 1);
                }
                else
                {
                    diff += (nums2[j++] - 6);
                }
            }
            else if (i < nums1.size())
            {
                if (nums1[i] == 1)
                    return -1;
                diff -= (nums1[i++] - 1);
            }
            else
            {
                if (nums2[j] == 6)
                    return -1;
                diff += (nums2[j++] - 6);
            }
            res++;
            if (diff <= 0)
                return res;
        }
        return -1;
    }

    int minOperations_v2(vector<int> &nums1, vector<int> &nums2)
    {
        int s1 = accumulate(nums1.begin(), nums1.end(), 0);
        int s2 = accumulate(nums2.begin(), nums2.end(), 0);
        if (s1 < s2)
        {
            swap(s1, s2);
            swap(nums1, nums2);
        }
        map<int, int, greater<int>> cnts1;
        map<int, int> cnts2;
        for (auto num : nums1)
            cnts1[num]++;
        for (auto num : nums2)
            cnts2[num]++;
        auto iter1 = cnts1.begin();
        auto iter2 = cnts2.begin();
        int diff = s1 - s2;
        if (diff == 0)
            return 0;
        int res = 0;
        while (iter1 != cnts1.end() || iter2 != cnts2.end())
        {
            if (iter1 != cnts1.end() && iter2 != cnts2.end())
            {
                if (iter1->first == 1 && iter2->first == 6)
                    return -1;
                if (iter1->first + iter2->first >= 7)
                {
                    diff -= (iter1->first - 1);
                    iter1->second--;
                    if (iter1->second == 0)
                        iter1++;
                }
                else
                {
                    diff += (iter2->first - 6);
                    iter2->second--;
                    if (iter2->second == 0)
                        iter2++;
                }
            }
            else if (iter1 != cnts1.end())
            {
                if (iter1->first == 1)
                    return -1;
                diff -= (iter1->first - 1);
                iter1->second--;
                if (iter1->second == 0)
                    iter1++;
            }
            else
            {
                if (iter1->second == 6)
                    return -1;
                diff += (iter2->first - 6);
                iter2->second--;
                if (iter2->second == 0)
                    iter2++;
            }
            res++;
            if (diff <= 0)
                return res;
        }
        return -1;
    }

    int help(vector<int> &h1, vector<int> &h2, int diff)
    {
        vector<int> h(7, 0);
        for (int i = 1; i < 7; ++i)
        {
            h[6 - i] += h1[i];
            h[i - 1] += h2[i];
        }
        int res = 0;
        for (int i = 5; i && diff > 0; --i)
        {
            int t = min((diff + i - 1) / i, h[i]);
            res += t;
            diff -= t * i;
        }
        return res;
    }

    int minOperations(vector<int> &nums1, vector<int> &nums2)
    {
        int n = nums1.size(), m = nums2.size();
        if (6 * n < m || 6 * m < n)
        {
            return -1;
        }
        vector<int> cnt1(7, 0), cnt2(7, 0);
        int diff = 0;
        for (auto &i : nums1)
        {
            ++cnt1[i];
            diff += i;
        }
        for (auto &i : nums2)
        {
            ++cnt2[i];
            diff -= i;
        }
        if (!diff)
        {
            return 0;
        }
        if (diff > 0)
        {
            return help(cnt2, cnt1, diff);
        }
        return help(cnt1, cnt2, -diff);
    }
};