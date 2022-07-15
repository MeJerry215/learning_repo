#include "common.h"

/*
给定两个大小分别为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。请你找出并返回这两个正序数组的 中位数 。
算法的时间复杂度应该为 O(log (m+n)) 。
 

示例 1：
输入：nums1 = [1,3], nums2 = [2]
输出：2.00000
解释：合并数组 = [1,2,3] ，中位数 2
示例 2：
输入：nums1 = [1,2], nums2 = [3,4]
输出：2.50000
解释：合并数组 = [1,2,3,4] ，中位数 (2 + 3) / 2 = 2.5
 
提示：
nums1.length == m
nums2.length == n
0 <= m <= 1000
0 <= n <= 1000
1 <= m + n <= 2000
-106 <= nums1[i], nums2[i] <= 106

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/median-of-two-sorted-arrays
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution
{
public:
    double findMedianSortedArrays2(vector<int> &nums1, vector<int> &nums2)
    {
        /*
        中位数， 即在有序n 长度数组中，小于中位数存在n / 2的元素， 大于中位数的元素存在 n/2个
        This implementation is O(m + n), not fullfill the requrements of performance O(log(m + n))
        */
        int size = nums1.size() + nums2.size();
        double before = 0, cur = 0;
        int i = 0, j = 0, k = 0;
        nums1.push_back(INT_MAX);
        nums2.push_back(INT_MAX);
        for (; i <= size / 2; i++)
        {
            before = cur;
            if (nums1[j] < nums2[k])
            {
                cur = nums1[j++];
            }
            else
            {
                cur = nums2[k++];
            }
        }
        cout << before << " " << cur << endl;
        if (size % 2 == 0)
        {
            return (before + cur) / 2;
        }
        else
        {
            return cur;
        }
    }

    // https://leetcode.cn/problems/median-of-two-sorted-arrays/solution/4-xun-zhao-liang-ge-you-xu-shu-zu-de-zhong-wei-shu/
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
		int n = nums1.size();
		int m = nums2.size();

		if (n > m)  //保证数组1一定最短
		{
			return findMedianSortedArrays(nums2, nums1);
		}

		// Ci 为第i个数组的割,比如C1为2时表示第1个数组只有2个元素。LMaxi为第i个数组割后的左元素。RMini为第i个数组割后的右元素。
		int LMax1, LMax2, RMin1, RMin2, c1, c2, lo = 0, hi = 2 * n;  //我们目前是虚拟加了'#'所以数组1是2*n长度

		while (lo <= hi)   //二分
		{
			c1 = (lo + hi) / 2;  //c1是二分的结果 #
			c2 = m + n - c1;

			LMax1 = (c1 == 0) ? INT_MIN : nums1[(c1 - 1) / 2];
			RMin1 = (c1 == 2 * n) ? INT_MAX : nums1[c1 / 2];
			LMax2 = (c2 == 0) ? INT_MIN : nums2[(c2 - 1) / 2];
			RMin2 = (c2 == 2 * m) ? INT_MAX : nums2[c2 / 2];

			if (LMax1 > RMin2)
				hi = c1 - 1;
			else if (LMax2 > RMin1)
				lo = c1 + 1;
			else
				break;
		}
		return (max(LMax1, LMax2) + min(RMin1, RMin2)) / 2.0;
	}

};

void TestSolution()
{
    Solution s;
    vector<int> v1 = {1};
    vector<int> v2 = {};
    cout << s.findMedianSortedArrays(v1, v2) << endl;
}