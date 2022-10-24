#include "common.h"

/*
给你一个整数数组 arr ，请使用 煎饼翻转 完成对数组的排序。
一次煎饼翻转的执行过程如下：
选择一个整数 k ，1 <= k <= arr.length
反转子数组 arr[0...k-1]（下标从 0 开始）
例如，arr = [3,2,1,4] ，选择 k = 3 进行一次煎饼翻转，反转子数组 [3,2,1] ，得到 arr = [1,2,3,4] 。
以数组形式返回能使 arr 有序的煎饼翻转操作所对应的 k 值序列。任何将数组排序且翻转次数在 10 * arr.length 范围内的有效答案都将被判断为正确。

 

示例 1：

输入：[3,2,4,1]
输出：[4,2,4,3]
解释：
我们执行 4 次煎饼翻转，k 值分别为 4，2，4，和 3。
初始状态 arr = [3, 2, 4, 1]
第一次翻转后（k = 4）：arr = [1, 4, 2, 3]
第二次翻转后（k = 2）：arr = [4, 1, 2, 3]
第三次翻转后（k = 4）：arr = [3, 2, 1, 4]
第四次翻转后（k = 3）：arr = [1, 2, 3, 4]，此时已完成排序。 
示例 2：

输入：[1,2,3]
输出：[]
解释：
输入已经排序，因此不需要翻转任何内容。
请注意，其他可能的答案，如 [3，3] ，也将被判断为正确。
 

提示：

1 <= arr.length <= 100
1 <= arr[i] <= arr.length
arr 中的所有整数互不相同（即，arr 是从 1 到 arr.length 整数的一个排列）

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/pancake-sorting
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

/*
pancake 排序，是对k位置进行逆序，由于k之前的位置总是变动的，每次计算都很麻烦，但是k之后的序列都是固定不动的
所以我们的思路将 数组分为两个部分，k之前的和k之后，k之后的一旦排序完之后就不会再动了。
3 2 4 1 k = 3
4 2 3 1 k = 4
1 3 2 4 k = 2
3 1 2 4 k = 3
2 1 3 4 k = 2
1 2 3 4 

所以对于任意数组只要每次将对应位置的元素转到最前面去，然后再转回对应的位置，即可再2 * n 次内完成排序。



*/

class Solution {
public:
    void rotate(vector<int>& arr, int i, int j) {
        while(i < j) {
            swap(arr[i++], arr[j--]);
        }
    }

    vector<int> pancakeSort(vector<int>& arr) {
        int j = arr.size() - 1;
        vector<int> res;
        while(j > 0) {
            if (arr[j] != j + 1) {
                int i = find(arr.begin(), arr.end(), j  + 1) - arr.begin();
                if (i != 0) {
                    rotate(arr, 0, i);
                    res.push_back(i + 1);
                }
                rotate(arr, 0, j);
                res.push_back(j + 1);
            }
            j--;
        }
        return res;
    }
};

void TestSolution() {
    vector<int> arr = {3,2,4,1};
    Solution s;
    auto res = s.pancakeSort(arr);
    print_vec(res);
}