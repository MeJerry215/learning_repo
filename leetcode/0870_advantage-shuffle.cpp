#include "common.h"

/*
给定两个大小相等的数组 nums1 和 nums2，nums1 相对于 nums2 的优势可以用满足 nums1[i] > nums2[i] 的索引 i 的数目来描述。
返回 nums1 的任意排列，使其相对于 nums2 的优势最大化。

示例 1：
输入：nums1 = [2,7,11,15], nums2 = [1,10,4,11]
输出：[2,11,7,15]
示例 2：
输入：nums1 = [12,24,8,32], nums2 = [13,25,32,11]
输出：[24,32,8,12]
提示：
 1 <= nums1.length <= 105
nums2.length == nums1.length
0 <= nums1[i], nums2[i] <= 109

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/advantage-shuffle
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/


/*
解题思路，只要nums1中选择比nums2中稍微大一点的数就行，至于比nums2中对应位置找不到更大的数，可以填之前nums1未匹配到的nums2中的小数。
这道题目唯一的难点在于对于nums2 是需要排序的，但是nums2排序之后就失去了索引信息，
所以这里需要绕一个弯，对nums2的索引排序，而不是对nums2直接排序。 所以这一步是真的妙。

这里我们知道了nums2的排序结果，以及元素在排序后所在的索引。

那么nums1 从小开始筛选，如果nums1 当前数大于 nums2 排序后的数，更新
否则，就将nums2最大的数，配给当前的小数，这里有一点田忌赛马的味道了。
*/
class Solution {
public:
    vector<int> advantageCount(vector<int>& nums1, vector<int>& nums2) {
        sort(nums1.begin(), nums1.end());

        // sort(nums2.begin(), nums2.end());
        vector<int> index_nums2(nums2.size(), 0);
        iota(index_nums2.begin(), index_nums2.end(), 0);
        sort(index_nums2.begin(), index_nums2.end(), [&](const int i, const int j) {
            return nums2[i] < nums2[j];
        });
        int low = 0, high = nums1.size() - 1;
        for(int i = 0; i < nums1.size(); i++) {
            if (nums1[i] > nums2[index_nums2[low]]) {
                nums2[index_nums2[low++]] = nums1[i];
            } else {
                nums2[index_nums2[high--]] = nums1[i];
            }
        }
        return nums2;
    }
};

void TestSolution() {
    Solution s;
    vector<int> nums1 = {2,7,11,15};
    vector<int> nums2 = {1,10,4,11};
    s.advantageCount(nums1, nums2);
    
}