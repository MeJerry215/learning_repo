# Design

程序设计类题，主要包含按照需求写出实现。



## 895. 最大频率栈

[895. 最大频率栈](https://leetcode.cn/problems/maximum-frequency-stack/)

```python
'''
设计一个类似堆栈的数据结构，将元素推入堆栈，并从堆栈中弹出出现频率最高的元素。
实现 FreqStack 类:
FreqStack() 构造一个空的堆栈。
void push(int val) 将一个整数 val 压入栈顶。
int pop() 删除并返回堆栈中出现频率最高的元素。
如果出现频率最高的元素不只一个，则移除并返回最接近栈顶的元素。

示例 1：

输入：
["FreqStack","push","push","push","push","push","push","pop","pop","pop","pop"],
[[],[5],[7],[5],[7],[4],[5],[],[],[],[]]
输出：[null,null,null,null,null,null,null,5,7,5,4]
解释：
FreqStack = new FreqStack();
freqStack.push (5);//堆栈为 [5]
freqStack.push (7);//堆栈是 [5,7]
freqStack.push (5);//堆栈是 [5,7,5]
freqStack.push (7);//堆栈是 [5,7,5,7]
freqStack.push (4);//堆栈是 [5,7,5,7,4]
freqStack.push (5);//堆栈是 [5,7,5,7,4,5]
freqStack.pop ();//返回 5 ，因为 5 出现频率最高。堆栈变成 [5,7,5,7,4]。
freqStack.pop ();//返回 7 ，因为 5 和 7 出现频率最高，但7最接近顶部。堆栈变成 [5,7,5,4]。
freqStack.pop ();//返回 5 ，因为 5 出现频率最高。堆栈变成 [5,7,4]。
freqStack.pop ();//返回 4 ，因为 4, 5 和 7 出现频率最高，但 4 是最接近顶部的。堆栈变成 [5,7]。
 
提示：
0 <= val <= 109
push 和 pop 的操作数不大于 2 * 104。
输入保证在调用 pop 之前堆栈中至少有一个元素。
'''

## 1570. 两个稀疏向量的点积
[1570. 两个稀疏向量的点积](https://leetcode.cn/problems/dot-product-of-two-sparse-vectors/)

```python
'''
给定两个稀疏向量，计算它们的点积（数量积）。

实现类 SparseVector：

SparseVector(nums) 以向量 nums 初始化对象。
dotProduct(vec) 计算此向量与 vec 的点积。
稀疏向量 是指绝大多数分量为 0 的向量。你需要 高效 地存储这个向量，并计算两个稀疏向量的点积。

进阶：当其中只有一个向量是稀疏向量时，你该如何解决此问题？

 

示例 1：

输入：nums1 = [1,0,0,2,3], nums2 = [0,3,0,4,0]
输出：8
解释：v1 = SparseVector(nums1) , v2 = SparseVector(nums2)
v1.dotProduct(v2) = 1*0 + 0*3 + 0*0 + 2*4 + 3*0 = 8
示例 2：

输入：nums1 = [0,1,0,0,0], nums2 = [0,0,0,0,2]
输出：0
解释：v1 = SparseVector(nums1) , v2 = SparseVector(nums2)
v1.dotProduct(v2) = 0*0 + 1*0 + 0*0 + 0*0 + 0*2 = 0
示例 3：

输入：nums1 = [0,1,0,0,2,0,0], nums2 = [1,0,0,0,3,0,4]
输出：6
 

提示：

n == nums1.length == nums2.length
1 <= n <= 10^5
0 <= nums1[i], nums2[i] <= 100

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/dot-product-of-two-sparse-vectors
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
'''
```