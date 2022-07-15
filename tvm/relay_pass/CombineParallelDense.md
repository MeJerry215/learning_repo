# CombineParallelDense

CombineParallelDense 和其他的 CombineParallelConv2D、CombineParallelBatchMatmul 都是对并行结构的Op进行融合，从而达到减少Op的目的，从而减少kernelLaunch 的时间。

当前Bert网络中AttentionLayer的典型配置，就是QKV三路FC， 然后到MultiHeadAttention中，通过将三路合并成一路减少op的数目。当前如果用tvm对bert进行量化，最后bert-base出来的op数目有接近 2000个，虽然tvm目前依赖其FuseOps能够减少一部分的Op 最后实测kernel launch的数目为400+， 而相较于TensorRT的优化bert，op数量可能只有70+。这些就属于体外话了。从目前来看TVM对于Bert网络的支持不是很好。

参考 [**[Discussion] New IR pass proposal: CombineParallelDense**](https://discuss.tvm.apache.org/t/discussion-new-ir-pass-proposal-combineparalleldense/3813) 

![image-20220714152151688](D:\Repo\learning_repo\tvm\relay_pass\CombineParallelDense.assets\image-20220714152151688.png)



OK，那么经过融合之后的图长什么样子呢，该Pass存在两种融合方式，一种是融合称为batch_matmul，另外一种是融合成dense。

而在这之后elemwise则成为一个op，等到后续分叉则会进行split或者是strideslice操作。

![image-20220714161901335](D:\Repo\learning_repo\tvm\relay_pass\CombineParallelDense.assets\image-20220714161901335.png)