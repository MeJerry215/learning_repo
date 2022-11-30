# Range Sum


https://leetcode.cn/problems/corporate-flight-bookings/solution/gong-shui-san-xie-yi-ti-shuang-jie-chai-fm1ef/
查分数组，线段树
[1109. 航班预订统计](https://leetcode.cn/problems/corporate-flight-bookings/)
[370. 区间加法](https://leetcode.cn/problems/range-addition/)

## 树状数组
单点更新 + 范围查询

树状数组/线段树

数状数组是一个查询和修改的复杂度都为O(n)的数据结构，主要用于数组的**单点修改和区间求和**，线段树同样具有此功能

树状数组和线段树的区别和联系如下：

1. 两者在复杂度上相似，树状数组明显优于线段树，编程复杂度小于线段树
2. 树状数组的作用能够完全被线段树覆盖，凡是可以使用线段树可以解决的问题都可以使用线段树解决。
3. 树状数组的核心在lowbit操作，代码效率高于线段树。

![tree_arr](https://pic4.zhimg.com/80/v2-e5c6401d88a3fa1a051232695568e26b_1440w.webp)



## 线段树


