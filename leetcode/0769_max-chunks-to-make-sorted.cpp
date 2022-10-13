#include "common.h"

/*
给定一个长度为 n 的整数数组 arr ，它表示在 [0, n - 1] 范围内的整数的排列。
我们将 arr 分割成若干 块 (即分区)，并对每个块单独排序。将它们连接起来后，使得连接的结果和按升序排序后的原数组相同。
返回数组能分成的最多块数量。

示例 1:
输入: arr = [4,3,2,1,0]
输出: 1
解释:
将数组分成2块或者更多块，都无法得到所需的结果。
例如，分成 [4, 3], [2, 1, 0] 的结果是 [3, 4, 0, 1, 2]，这不是有序的数组。
示例 2:
输入: arr = [1,0,2,3,4]
输出: 4
解释:
我们可以把它分成两块，例如 [1, 0], [2, 3, 4]。
然而，分成 [1, 0], [2], [3], [4] 可以得到最多的块数。

提示:
n == arr.length
1 <= n <= 10
0 <= arr[i] < n
arr 中每个元素都 不同
*/

class Solution
{
public:
    int maxChunksToSorted(vector<int>& arr) {
        /*
        由于数组range范围在[0, n-1], 且unique。
        则排序好后的value和索引是对应的
        3, 1, 2, 0, 4, 6, 7, 5
        0，1, 2, 3, 4, 5, 6, 7
        应该是使用greedy方法，将已经可以成为chunk的划分到一组上去。
        而如何称为一个有效的chunk呢?
        chunk内的最大和最小索引之间的序列是连续，且存在最大索引减去最小索引 + 1个数
        */
        int res = 0;
        int i = 0;

        while (i < arr.size()) {
            int low = i, high = i;
            int min_val = arr[i], max_val = arr[i];

            while (high < arr.size()) {
                min_val = min(min_val, arr[high]);
                max_val = max(max_val, arr[high]);

                if (min_val == low && max_val == high)
                { break; }

                cout << "iter " << high << " now " << min_val << ", " << max_val << "  low " << low << " high " << high << endl;
                high++;
            }

            cout << "range in " << low << " " << high << endl;
            i = high + 1;
            res++;
        }

        return res;
    }
};

int main(int argc, char** argv)
{
    Solution s;
    vector<int> arr = {3, 1, 2, 0, 4, 6, 7, 5};
    cout << s.maxChunksToSorted(arr);
}