# Cuda Samples

基于cuda优化并行规约

- CPU和GPU地址空间分开
- GPU中的内存都是锁页内存, 锁页内存存放的内容在任何情况下都不会与主机的虚拟内存进行交换(注:虚拟内存就是硬盘)
- 统一寻址(UVA)， 支持UVA的系统会自动启动UVA
- 锁页主机内存：GPU可以直接访问的CPU内存
- 命令缓冲区： 由CUDA驱动程序写入命令，CPU从缓冲区读取命令并控制执行。
- CPU/GPU同步： 指CPU如何追踪GPU的进度
- GPU可以通过DMA(Direct Memory Access)访问CPU中的锁页内存。
- GPU受限、CPU受限。GPU受限：CPU在写入下一条指令时，需要等到GPU执行完，CPU受限：GPU一直在等待CPU的下一条指令。
- CUDA内核程序的启动都是异步的。

一个GPU可以简化为：
- 一个连接GPU和PCIe总线的主机接口(读取GPU命令并派发)
- 0-2个复制引擎
- 一个链接GPU与其设备的内存的DRAM接口(包含用于合并内存请求的硬件，支持超过100GB/s的带宽)
- 一定数目的TPC或者GPC(纹理处理集群和图形处理集群)，每个包含一定的缓存和流处理簇(SMs)



每个TPC/GPC包含若干个SM

每个SM包含：
- 执行单元，执行32位整数和单、双精度浮点数运算(SP)
- 特殊函数单元(SFU)，用来计算log/exp, sin/cos, rcp/rqrst单精度近似值
- 一个线程束调度器，用以协调把指令发送到执行单元
- 常量缓存，用来广播室传送数据给SM
- 共享内存，用于线程之间的数据交换(shared memory)
- 纹理映射的专用硬件
- 寄存器文件(Registeer)
- 一级缓存(一级缓存可能和共享内存共享)/二级缓存


## 编译toolchaine

nvcc 是编译机器驱动程序，将cuda源代码转换为可执行cuda应用程序，可以用来把编译、链接等操作，编译一个旨在GPU上执行的.cu文件。

nvcc只在特定的编译器上工作，nvcc需要和生成环兼容，nvcc同时提供一些非标准内建类型(int2等)和固有名称(__popc()) 函数。 这些函数和数据类型可以通过适当的引入`sm_*_intriscics.h` 文件引入
![[cuda_samples/pics/Pasted image 20221220204846.png]]

cubin 文件可以使用`cuobjdump --dump-sass`进行反汇编生成SASS代码。