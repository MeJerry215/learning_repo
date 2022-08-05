# Quantization

[模型量化（int8）知识梳理](https://zhuanlan.zhihu.com/p/516116539)



常用的量化方式主要是线性量化：对称量化和非对称量化

量化粒度：per-tensor，per-channel，量化粒度越小，精度越好，成本越高

量化：float32 -> int8 的过程，计算公式如下，当b为0的时候就是对称量化，看是对称量化还是非对称量化主要就是看是否存在零点。
$$
y = w * x + b
$$


反量化：int8 -> float32的过程

对于量化：对称量化可以满足绝大部分的需求，weight量化非对称量化比对称量化代价大，激活函数的量化一般使用非对称量化，或者成uint8。

![非对称量化和对称量化](https://img-blog.csdnimg.cn/img_convert/3e1797c9e43ac8f9bc223718a589d505.png)

在非对称量化场景下，$\alpha$和$\beta$是相等的，如果要从float量化到int8则需要计算$w$和$b$
$$
w = \frac{(\alpha - \beta)}{255} \\
b = 
$$
