# lower

lower 是te表达式或者是tir表达式，生成更低层次表达形式如代码指令。
通常来说我们使用TE定义了计算表达式，然后生成schedule，将schedule lower为目标平台的指令代码。


```python

k = te.reduce_axis((0, K), "k")
A = te.placeholder((M, K), name="A")
B = te.placeholder((K, N), name="B")
C = te.compute((M, N), lambda m, n: te.sum(A[m, k] * B[k, n], axis=k), name="C")
s = te.create_schedule(C.op)
```

上面定义了矩阵乘法操作，然后创建了一个schedule，然后去lower。

所以我们需要对TE层Op有一个简单的了解存在哪些Op。 Op定义在`include/tvm/te/operation.h`, 常见的Op有：
- 