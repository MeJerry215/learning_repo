# Jump Game

跳跃游戏在LeetCode 一共存在7题目，对于所使用的解法也存在差异，目前看每题的解法大致为DFS、BFS、贪心、DP、DP扩展，所以将这一系列题目做完能够学到挺多东西

[55. 跳跃游戏](https://leetcode.cn/problems/jump-game/)
[45. 跳跃游戏 II](https://leetcode.cn/problems/jump-game-ii/)
[1306. 跳跃游戏 III](https://leetcode.cn/problems/jump-game-iii/)
[1345. 跳跃游戏 IV](https://leetcode.cn/problems/jump-game-iv/)
[1340. 跳跃游戏 V](https://leetcode.cn/problems/jump-game-v/)
[1696. 跳跃游戏 VI](https://leetcode.cn/problems/jump-game-vi/)
[1871. 跳跃游戏 VII](https://leetcode.cn/problems/jump-game-vii/)

如果让我为这集体排序的话，我的排序是

`I < II < III < VII <  V < VI < IV`

所以从我的理解的难易度顺序开始看看每题是什么情况

## 跳跃游戏

```c++
/*
给定一个非负整数数组 nums ，你最初位于数组的 第一个下标 。
数组中的每个元素代表你在该位置可以跳跃的最大长度。
判断你是否能够到达最后一个下标。

示例 1：
输入：nums = [2,3,1,1,4]
输出：true
解释：可以先跳 1 步，从下标 0 到达下标 1, 然后再从下标 1 跳 3 步到达最后一个下标。
示例 2：

输入：nums = [3,2,1,0,4]
输出：false
解释：无论怎样，总会到达下标为 3 的位置。但该下标的最大跳跃长度是 0 ， 所以永远不可能到达最后一个下标。

提示：
1 <= nums.length <= 3 * 104
0 <= nums[i] <= 105
*/
```

分析一下， 从0位置开始，看能否道最后的位置，一个很容易的想法就是我在当前位置能够跑的最远的距离是哪里，在能够跳到的最远距离之前，我都是可以随便跑的。

大炮射程之内即真理，所以，根据以上分析在当前位置i, 我在当前位置可以跳跃的最远距离为之前的最远距离 or 当前位置起跳的最远距离, 只选择跳的最远的(贪心)。

 $farest[i] = max(farest[i - 1], i + nums[i])$

```c++
class Solution
{
public:
    bool canJump_v1(vector<int> &nums)
    {
        int n = nums.size();
        vector<int> farest(n, 0);
        farest[0] = nums[0];
        for (int i = 1; i < nums.size(); i++)
        {
            if (i < farest[i - 1]) return false;
            farest[i] = max(farest[i - 1], i + nums[i]);
            if (farest[i] >= n - 1) return true;
        }
        return true;
    }

    bool canJump_opt(vector<int> &nums)
    {
        int n = nums.size();
        int step = 0;
        for (int i = 0; i < n; i++)
        {
            if (step < i) return false;
            step = max(step, i + nums[i]);
            if (step >= n - 1) return true;
        }
        return true;
    }
};

```

## 跳跃游戏 II

```c++
/*
给你一个非负整数数组 nums ，你最初位于数组的第一个位置。
数组中的每个元素代表你在该位置可以跳跃的最大长度。
你的目标是使用最少的跳跃次数到达数组的最后一个位置。
假设你总是可以到达数组的最后一个位置。

示例 1:
输入: nums = [2,3,1,1,4]
输出: 2
解释: 跳到最后一个位置的最小跳跃数是 2。
     从下标为 0 跳到下标为 1 的位置，跳 1 步，然后跳 3 步到达数组的最后一个位置。
示例 2:
输入: nums = [2,3,0,1,4]
输出: 2

提示:
1 <= nums.length <= 104
0 <= nums[i] <= 1000
*/
```

跳跃的最小次数，也就是说也每次都跳的远，用的次数少，且题目保证必然可以跳到最后一个位置。

则在当前能够跳到的位置下一次位置范围内，选择下一次跳的比较远的。依旧是贪心


```c++
class Solution
{
public:
    int jump(vector<int> &nums) {
        int max_pos = 0, end = 0, step = 0;
        for (int i = 0; i < nums.size() - 1; ++i) {
            max_pos = max(max_pos, i + nums[i]);
            if (i == end) {
                end = max_pos;
                ++step;
            }
        }
        return step;
    }
};

```


###  跳跃游戏 III

```c++
这里有一个非负整数数组 arr，你最开始位于该数组的起始下标 start 处。当你位于下标 i 处时，你可以跳到 i + arr[i] 或者 i - arr[i]。
请你判断自己是否能够跳到对应元素值为 0 的 任一 下标处。
注意，不管是什么情况下，你都无法跳到数组之外。

示例 1：
输入：arr = [4,2,3,0,3,1,2], start = 5
输出：true
解释：
到达值为 0 的下标 3 有以下可能方案： 
下标 5 -> 下标 4 -> 下标 1 -> 下标 3 
下标 5 -> 下标 6 -> 下标 4 -> 下标 1 -> 下标 3 
示例 2：
输入：arr = [4,2,3,0,3,1,2], start = 0
输出：true 
解释：
到达值为 0 的下标 3 有以下可能方案： 
下标 0 -> 下标 4 -> 下标 1 -> 下标 3
示例 3：
输入：arr = [3,0,2,1,2], start = 2
输出：false
解释：无法到达值为 0 的下标 1 处。 

提示：

1 <= arr.length <= 5 * 10^4
0 <= arr[i] < arr.length
0 <= start < arr.length
```


