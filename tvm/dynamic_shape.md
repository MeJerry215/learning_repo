# Dynamic Shape

以下列举可能支持动态shape的方案：
- TVM Virtual Machine，[Putting the VM in TVM: The Relay Virtual Machine](https://tvm.apache.org/docs/arch/virtual_machine.html)
- [The CoRa Tensor Compiler: Compilation for Ragged Tensors with Minimal Padding](https://arxiv.org/abs/2110.10221)
- [DIETCODE: AUTOMATIC OPTIMIZATION FOR DYNAMIC TENSOR PROGRAMS](https://proceedings.mlsys.org/paper/2022/file/fa7cdfad1a5aaf8370ebeda47a1ff1c3-Paper.pdf)
- [NIMBLE: EFFICIENTLY COMPILING DYNAMIC NEURAL NETWORKS FOR MODEL INFERENCE](https://arxiv.org/pdf/2006.03031.pdf)
## TVM Virtual Machine

...


## THE CORA TENSOR COMPILER: COMPILATION FOR RAGGED TENSORS WITH MINIMAL PADDING

Problem: often variation in the shape and the size of input data(non-uniform shape or ragged tensors). often using padding and masking to make data shapes uniforms. leads to waste computation.
- C1: 代码生成的不规则性：循环边界导致代码不规则，导致GPU性能下降
- C2: 编译器机制不足，循环和张量维度之间一拉，所以在循环和切片的张量维度转换不容易，同时稀疏使得问题复杂。
- C3: 计算抽象的不合理性

Solution: **Co**mpiler for **Ra**ngged Tensor.
![[tvm/pics/Pasted image 20230207160313.png]]
- Cora 最小化padding来生成高效的GPU代码
- extending tvm v0.6(Chen et al.., 2018a)
-  Our implementation currently expects users to correctly  allocate memory (taking into account padding requirements  as specified in the schedule) for tensors.
- where the FT-Eff implementation employs full  padding while the CORA employs partial padding.
![[tvm/pics/Pasted image 20230207155954.png]]

## DIETCODE: AUTOMATIC OPTIMIZATION FOR   DYNAMIC TENSOR PROGRAMS

Problem:
- search space is shape-dependent
- existing auto-schedulers require workloads to be static to perform analysis
- compilation time extremely long

Solution: 
- a new auto-scheduler framework that efficiently supports dynamic-shape workloads by constructing a shape-generic search space that is made up of **micro-kernels**,  incomplete programs that carries out a tile of the complete computation. Because every **micro-kernel** can be ported to every shape of the workload, this gives DietCode a unified space to search for efficient schedules for all the shapes.
- apply existing auto-schedulers directly to dynamic-shape workloads, an efficient and practical solution for dynamic-shape workloads.
- efficiently supports dynamic-shape workloads by constructing a shape-generic search space and cost model
- using local padding that automatically pads the local workspace when fetching the input tensors from global memory.
- a  micro-kernel M can be decomposed into two parts: (1) a shape-generic cost function fMK that predicts the cost of  M , and (2) a shape-dependent adaption cost function fadapt  that defines the penalty of porting M to P .
![[tvm/pics/Pasted image 20230208094958.png]]
![[tvm/pics/Pasted image 20230208101015.png]]
![[tvm/pics/Pasted image 20230208101327.png]]
![[tvm/pics/Pasted image 20230208102251.png]]

## NIMBLE: EFFICIENTLY COMPILING DYNAMIC NEURAL NETWORKS FOR  MODEL INFERENCE
problem:
- missing an IR for representing dynamism
- lack of a set of dynamic-oriented optimizations. static memory pre-allocated, static pass dependent on shape information.
- challenges symbolic kernel code generator and kernel tuning under  symbolic shape.
solution:
- An extended type system which enables static tracking of  dynamic shapes. A series of optimization passes that make dynamic output. set of codegen techniques for producing code of kernels  
with dynamic input and output shapes.
shapes, allocation, and device placement explicit
- two major components: a dynamic compiler with various analysis  
and optimization techniques and a VM-based runtime.
![[tvm/pics/Pasted image 20230209114336.png]]