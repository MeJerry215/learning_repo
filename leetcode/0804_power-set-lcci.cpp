#include "common.h"

/*
幂集。编写一种方法，返回某集合的所有子集。集合中不包含重复的元素。

说明：解集不能包含重复的子集。

示例:

 输入： nums = [1,2,3]
 输出：
[
  [3],
  [1],
  [2],
  [1,2,3],
  [1,3],
  [2,3],
  [1,2],
  []
]

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/power-set-lcci
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
class Solution
{
public:
  vector<vector<int>> subsets(vector<int> &nums)
  {
    int n = nums.size();
    int res_num = 0;
    for (int i = 0; i < nums.size(); i++)
    {
      res_num *= 2;
      res_num += 1;
    }
    vector<vector<int>> res(res_num + 1, vector<int>());
    for (int mask = 0; mask <= res_num; mask++)
    {
      vector<int> &subset = res[mask];
      int i = 0;
      int tmp = mask;
      while (tmp)
      {
        if (tmp % 2)
        {
          subset.push_back(nums[i]);
        }
        i++;
        tmp /= 2;
      }
    }
    return res;
  }
};