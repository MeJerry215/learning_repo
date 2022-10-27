# Summary

本文主要将刷题进行归类。

## 排序

[853. 车队](https://leetcode.cn/problems/car-fleet/)

## BFS

考察基本的BFS算法熟悉程度, 矩阵中BFS模板写法。
[面试题 08.10. 颜色填充](https://leetcode.cn/problems/color-fill-lcci/)

## DFS



## 快慢指针

判断是否成环
[457. 环形数组是否存在循环](https://leetcode.cn/problems/circular-array-loop/)


## 贪心

[862. 和至少为 K 的最短子数组](https://leetcode.cn/problems/shortest-subarray-with-sum-at-least-k/)


## 单调栈

单调栈性质：
1、若是单调递增栈，则从栈顶到栈底的元素是严格递增的。若是单调递减栈，则从栈顶到栈底的元素是严格递减的。
2、越靠近栈顶的元素越后进栈。

用法：单调增栈、单调减栈、前向单调栈、后向单调栈。

## 双端队列

双端队列：双端队列是指允许两端都可以进行入队和出队操作的队列，其元素的逻辑结构仍是线性结构。将队列的两端分别称为前端和后端，两端都可以入队和出队。

单调队列，顾名思义，就是一个元素单调的队列，那么就能保证队首的元素是最小（最大）的，从而满足最优性问题的需求。


本题维护单调增队列，当当前元素减去队首元素满足条件时，即获得一个解，而在此基础上，贪心，求得更小的解，出栈队首元素。
而当当前元素小于队尾元素时，说明队尾元素不在最优解考虑范围内。
[862. 和至少为 K 的最短子数组](https://leetcode.cn/problems/shortest-subarray-with-sum-at-least-k/)

## 前缀和/前缀积/后缀和/后缀积

前缀和从开头到某一位位置的和，而用两个和相减，则可以达到范围的和，避免了重复计算。


利用前缀和求和
[862. 和至少为 K 的最短子数组](https://leetcode.cn/problems/shortest-subarray-with-sum-at-least-k/)
