# Summary

本文主要将刷题进行归类。

## 模拟

常规思路是模拟，进阶思路是原地交换
[1528. 重新排列字符串](https://leetcode.cn/problems/shuffle-string/)


## 字符串

一道处理起来还挺麻烦的字符串题目
[6239. 根据限制分割消息](https://leetcode.cn/problems/split-message-based-on-limit/)

暴力解法的话可以直接substr
[1910. 删除一个字符串中所有出现的给定子字符串](https://leetcode.cn/problems/remove-all-occurrences-of-a-substring/)

## 字典树

[212. 单词搜索 II](https://leetcode.cn/problems/word-search-ii/)

## 分治

巧妙的一次遍历引用指针，由叶节点先增加引用，然后最后就是根节点，自然的到达中间值
[109. 有序链表转换二叉搜索树](https://leetcode.cn/problems/convert-sorted-list-to-binary-search-tree/)

[https://pan.baidu.com/s/1hjwK0ZeRxYGB8lIkbKuQgQ#list/path=%2Fsharelink3641294814-120028191352976%2F%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84%E5%92%8C%E7%AE%97%E6%B3%95&parentPath=%2Fsharelink3641294814-120028191352976](https://pan.baidu.com/s/1hjwK0ZeRxYGB8lIkbKuQgQ#list/path=%2Fsharelink3641294814-120028191352976%2F%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84%E5%92%8C%E7%AE%97%E6%B3%95&parentPath=%2Fsharelink3641294814-120028191352976)
6666

## 数组

逆序原地旋转字符串，三次reverse即可, 189为右旋
[剑指 Offer 58 - II. 左旋转字符串](https://leetcode.cn/problems/zuo-xuan-zhuan-zi-fu-chuan-lcof/)
[189. 轮转数组](https://leetcode.cn/problems/rotate-array/)

## 贡献值

一般用于求子串的个数，当前字符对于子串的贡献值有多大，就存在多少种该情况下的子串，分别求每个字符的贡献值求和。
[907. 子数组的最小值之和](https://leetcode.cn/problems/sum-of-subarray-minimums/)

[2348. 全 0 子数组的数目](https://leetcode.cn/problems/number-of-zero-filled-subarrays/)


[2393. Count Strictly Increasing Subarrays](https://leetcode.cn/problems/count-strictly-increasing-subarrays/)

## 栈

**设计**:这道题目可以在开始dfs掉所有元素，但是不符合迭代器的设计思路，迭代器延迟加载数据，所以方法二采用栈来模拟延迟加载，记录当前遍历到的元素iterator，以及尾iterator。
[341. 扁平化嵌套列表迭代器](https://leetcode.cn/problems/flatten-nested-list-iterator/)

栈的用法
[1106. 解析布尔表达式](https://leetcode.cn/problems/parsing-a-boolean-expression/)

出入栈即可
[1047. 删除字符串中的所有相邻重复项](https://leetcode.cn/problems/remove-all-adjacent-duplicates-in-string/)

## 排序

[853. 车队](https://leetcode.cn/problems/car-fleet/)

## 二分搜索
通常用来求解最大值最小化问题，最小值最大化问题。

[1011. 在 D 天内送达包裹的能力](https://leetcode.cn/problems/capacity-to-ship-packages-within-d-days/)
最大值最小化问题，直接二分搜索, 也可以使用动态规划求解
[410. 分割数组的最大值](https://leetcode.cn/problems/split-array-largest-sum/)
[875. 爱吃香蕉的珂珂](https://leetcode.cn/problems/koko-eating-bananas/)

## 二分查找

二分查找 + 容斥原理
![img](https://pic.leetcode.cn/1669032532-GjXsyF-878-2.png) 
[878. 第 N 个神奇数字](https://leetcode.cn/problems/nth-magical-number/)

## BFS

考察基本的BFS算法熟悉程度, 矩阵中BFS模板写法。
[面试题 08.10. 颜色填充](https://leetcode.cn/problems/color-fill-lcci/)

考察数的层次化遍历，这里不需要开辟queue来存储遍历的顺序，Node节点的next指针天然的就是遍历顺序。每一次都是处理下一层的next指针, 解法一致。
[116. 填充每个节点的下一个右侧节点指针](https://leetcode.cn/problems/populating-next-right-pointers-in-each-node/)
[117. 填充每个节点的下一个右侧节点指针 II](https://leetcode.cn/problems/populating-next-right-pointers-in-each-node-ii/)

[752. 打开转盘锁](https://leetcode.cn/problems/open-the-lock/)

[542. 01 矩阵](https://leetcode.cn/problems/01-matrix/)
## DFS

朴素dfs
[51. N 皇后](https://leetcode.cn/problems/n-queens/)

朴素dfs即可，模板化编程
[784. 字母大小写全排列](https://leetcode.cn/problems/letter-case-permutation/)

数dfs高度，需要求解到剩余的高度
[6223. 移除子树后的二叉树高度](https://leetcode.cn/problems/height-of-binary-tree-after-subtree-removal-queries/)

递归交换左右子树
[2415. 反转二叉树的奇数层](https://leetcode.cn/problems/reverse-odd-levels-of-binary-tree/)

组合系列，相对来说难度都是普通的dfs的变种
[39. 组合总和](https://leetcode.cn/problems/combination-sum/)
[40. 组合总和 II](https://leetcode.cn/problems/combination-sum-ii/)
[216. 组合总和 III](https://leetcode.cn/problems/combination-sum-iii/)
[46. 全排列](https://leetcode.cn/problems/permutations/)
[47. 全排列 II](https://leetcode.cn/problems/permutations-ii/)
[78. 子集](https://leetcode.cn/problems/subsets/)
[90. 子集 II](https://leetcode.cn/problems/subsets-ii/)


## 快慢指针

快指针2步，慢指针一步，最终相遇，然后推导 得到a+(n+1)b+nc=2(a+b)⟹a=c+(n−1)(b+c)
[!img](https://assets.leetcode-cn.com/solution-static/142/142_fig1.png)
[141. 环形链表](https://leetcode.cn/problems/linked-list-cycle/)
[142. 环形链表 II](https://leetcode.cn/problems/linked-list-cycle-ii/)

判断是否成环
[457. 环形数组是否存在循环](https://leetcode.cn/problems/circular-array-loop/)

删除倒数节点，让快指针先走k步，慢指针再开始和快指针一起走。
[19. 删除链表的倒数第 N 个结点](https://leetcode.cn/problems/remove-nth-node-from-end-of-list/)

## 滑动窗口/双指针

这到题目直接用滑动窗口去做是不太好做的，但是如果带上条件约束做滑动窗口还是可以做的，就是约束出现k次以上的字符的个数约束。
[395. 至少有 K 个重复字符的最长子串](https://leetcode.cn/problems/longest-substring-with-at-least-k-repeating-characters/)

固定大小窗口
[438. 找到字符串中所有字母异位词](https://leetcode.cn/problems/find-all-anagrams-in-a-string/)
438 稍微改改就能ac 567
[567. 字符串的排列](https://leetcode.cn/problems/permutation-in-string/)

滑动窗口统计字符个数，不断收缩扩张左右边界
[76. 最小覆盖子串](https://leetcode.cn/problems/minimum-window-substring/)


[167. 两数之和 II - 输入有序数组](two-sum-ii-input-array-is-sorted)

[1004. 最大连续1的个数 III](https://leetcode.cn/problems/max-consecutive-ones-iii/)
## 优先权队列

[23. 合并K个升序链表](https://leetcode.cn/problems/merge-k-sorted-lists/)

## 单调栈

单调栈性质：
1、若是单调递增栈，则从栈顶到栈底的元素是严格递增的。若是单调递减栈，则从栈顶到栈底的元素是严格递减的。
2、越靠近栈顶的元素越后进栈。

用法：单调增栈、单调减栈、前向单调栈、后向单调栈。

[907. 子数组的最小值之和](https://leetcode.cn/problems/sum-of-subarray-minimums/)


[496. 下一个更大元素 I](https://leetcode.cn/problems/next-greater-element-i/)
[503. 下一个更大元素 II](https://leetcode.cn/problems/next-greater-element-ii/)
556 并不是，这是类似于 [31. 下一个排列](https://leetcode.cn/problems/next-permutation/)
[556. 下一个更大元素 III](https://leetcode.cn/problems/next-greater-element-iii/)
[6227. 下一个更大元素 IV](https://leetcode.cn/problems/next-greater-element-iv/)

## 双端队列

双端队列：双端队列是指允许两端都可以进行入队和出队操作的队列，其元素的逻辑结构仍是线性结构。将队列的两端分别称为前端和后端，两端都可以入队和出队。

单调队列，顾名思义，就是一个元素单调的队列，那么就能保证队首的元素是最小（最大）的，从而满足最优性问题的需求。


本题维护单调增队列，当当前元素减去队首元素满足条件时，即获得一个解，而在此基础上，贪心，求得更小的解，出栈队首元素。
而当当前元素小于队尾元素时，说明队尾元素不在最优解考虑范围内。
[862. 和至少为 K 的最短子数组](https://leetcode.cn/problems/shortest-subarray-with-sum-at-least-k/)

## 前缀和/前缀积/后缀和/后缀积/差分

前缀和从开头到某一位位置的和，而用两个和相减，则可以达到范围的和，避免了重复计算。

前缀和可以有：一维前缀和、二前缀和

利用前缀和求和
[303. 区域和检索 - 数组不可变](https://leetcode.cn/problems/range-sum-query-immutable/)
[862. 和至少为 K 的最短子数组](https://leetcode.cn/problems/shortest-subarray-with-sum-at-least-k/)
[1109. 航班预订统计](https://leetcode.cn/problems/corporate-flight-bookings/)

状态压缩，bit运算，前缀和，非常有意思的一道题目。
[1177. 构建回文串检测](https://leetcode.cn/problems/can-make-palindrome-from-substring/)


6251 为长度5的回文子序列，1930使用bit count
[1930. 长度为 3 的不同回文子序列](https://leetcode.cn/problems/unique-length-3-palindromic-subsequences/)
[6251. 统计回文子序列数目](https://leetcode.cn/problems/count-palindromic-subsequences/)
## 贪心

[334. 递增的三元子序列](https://leetcode.cn/problems/increasing-triplet-subsequence/)
[862. 和至少为 K 的最短子数组](https://leetcode.cn/problems/shortest-subarray-with-sum-at-least-k/)

贪心最远距离，维护选择的每次跳跃的最后位置，超过了则需要增加跳跃次数
[45. 跳跃游戏 II](https://leetcode.cn/problems/jump-game-ii/)

## 动态规划

[55. 跳跃游戏](https://leetcode.cn/problems/jump-game/)

万物皆可动态规划
[1668. 最大重复子字符串](https://leetcode.cn/problems/maximum-repeating-substring/)

[322. 零钱兑换](https://leetcode.cn/problems/coin-change/)

状态有点多，搞清楚初始状态以及状态转移，轻松解决
[552. 学生出勤记录 II](https://leetcode.cn/problems/student-attendance-record-ii/)

[198. 打家劫舍](https://leetcode.cn/problems/house-robber/)
[213. 打家劫舍 II](https://leetcode.cn/problems/house-robber-ii/)
[337. 打家劫舍 III](https://leetcode.cn/problems/house-robber-iii/)

经典, 参考 https://leetcode.cn/problems/domino-and-tromino-tiling/solution/by-endlesscheng-umpp/
[790. 多米诺和托米诺平铺](https://leetcode.cn/problems/domino-and-tromino-tiling/)
[799. 香槟塔](https://leetcode.cn/problems/champagne-tower/)
[805. 数组的均值分割](https://leetcode.cn/problems/split-array-with-same-average/)

字符串dp, 记忆
[139. 单词拆分](https://leetcode.cn/problems/word-break/)
[140. 单词拆分 II](https://leetcode.cn/problems/word-break-ii/)

概率问题，记忆化dfs 或者dp都可以解
[808. 分汤](https://leetcode.cn/problems/soup-servings/)

记忆dp, 切木条二维情况
[2312. 卖木头块](https://leetcode.cn/problems/selling-pieces-of-wood/)

[542. 01 矩阵](https://leetcode.cn/problems/01-matrix/)

[813. 最大平均值和的分组](https://leetcode.cn/problems/largest-sum-of-averages/)

## 数学

到达重点  1 + ... +n 到达m位置，如果m距离target为偶数，翻转前面的一个 (target - m) / 2，1 + .... - (target -m) / 2 + ...n = m - (m - target), 若是剩下的看是否剩下的步数为奇数，目标是将步数转换为偶数。
[754. 到达终点数字](https://leetcode.cn/problems/reach-a-number/solution/shu-xue-zhu-shi-chao-ji-xiang-xi-by-xun-aethb/)

[2327. 知道秘密的人数](https://leetcode.cn/problems/number-of-people-aware-of-a-secret/)


9进制计算
[660. 移除 9](https://leetcode.cn/problems/remove-9/)



## 状态机
DP Table

[121. 买卖股票的最佳时机](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock/)
[122. 买卖股票的最佳时机 II](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-ii/)
[123. 买卖股票的最佳时机 III](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-iii/)
[188. 买卖股票的最佳时机 IV](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-iv/)
[309. 最佳买卖股票时机含冷冻期](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-with-cooldown/)
[714. 买卖股票的最佳时机含手续费](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-with-transaction-fee/)


## 设计类题

[895. 最大频率栈](https://leetcode.cn/problems/maximum-frequency-stack/)
