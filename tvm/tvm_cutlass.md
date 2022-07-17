# TVM CUTLASS

CUTLASS 是CUDA C++ 抽象模板实现高性能矩阵乘法库，类似于cuBlas和cuDNN。CUTLASS主要将可重用代码模块抽象成模板类。主要在`thread-wide`、`block-wide`、`device-wide`来达到不同尺度上kernel定制。

Cutlass实现高性能卷积通过implicit gemm 算法。在resnet50，cutlass的卷积性能也和cudnn性能，甚至部分能够超过cudnn。对于常自定义的激活层，可以自定义cutlass实现Epliogue operator获取新的卷积融合卷子。

![img](D:\Repo\learning_repo\tvm\tvm_cutlass.assets\v2-f02c898ff960bff8322ae90e58907ec2_1440w.jpg)

参考[[RFC\][BYOC]NVIDIA CUTLASS Integration](https://discuss.tvm.apache.org/t/rfc-byoc-nvidia-cutlass-integration/9147)

在TVM中引入Cutlass主要是相比tvm的auto scheduler相比cutlass存在巨大的性能gap。通过将cutlass集成进tvm，获得如下收益：

1. 对于gemm/conv kernel，达到80% 以上cublas相当性能。
2. cutlass支持算子融合，而cublas不支持
3. 支持各种data types
4. 能够预先编译出lib，提供各种dynamic shape支持。

对于TVM中使用cutlass主要是使用以下接口：`partition_for_cutlass`、`tune_cutlass_kernels`、`build_cutlass_kernels`、`build_cutlass_kernels_vm`

- partition_for_cutlass: 校验能够使用cutlass backend编译器op进行check，同时打上标记，对于后续提取出op的信息，然后profile选择出cutlass较优的实现。
- tune_cutlass_kernels：对于已经partion后的module，profile 并选择出较优的kernel emit代码。
- build_cutlass_kernels：编译cutlass kernel emit的代码成so，以便后续调用
- build_cutlass_kernels_vm： 动态shape支持

## partition_for_cutlass

**API**

```python
def partition_for_cutlass(mod, params=None) -> IRModule():
    '''
    Parameters
    ----------
    mod : IRModule
        The origin Relay module.
    params: params
   	Returns
    -------
    mod : IRModule
        The Relay module with cutlass partitions.
    '''
```

参数: mod, params

mod：即原始mod

params: 需要绑定的参数

返回经过partion标记过后的module

在调用`partition_for_cutlass`， 对于单op dense来说， 原来的relay ir如下：

```
def @main(%data: Tensor[(96, 64), float16], %weight: Tensor[(64, 64), float16]) -> Tensor[(96, 64), float32] {
  nn.dense(%data, %weight, units=None, out_dtype="float32") /* ty=Tensor[(96, 64), float32] */
}
```

经过partion之后，生成单独的cutlass dense函数，同时在main函数调用了dense func，生成的func中存在`cutlass`字段。

```
def @main(%data: Tensor[(96, 64), float16], %weight: Tensor[(64, 64), float16]) -> Tensor[(96, 64), float32] {
  @tvmgen_default_cutlass_main_0(%data, %weight) /* ty=Tensor[(96, 64), float32] */
}

def @tvmgen_default_cutlass_main_0(%cutlass_0_i0: Tensor[(96, 64), float16], %cutlass_0_i1: Tensor[(64, 64), float16], Inline=1, Compiler="cutlass", global_symbol="tvmgen_default_cutlass_main_0", Primitive=1) -> Tensor[(96, 64), float32] {
  %0 = fn (%FunctionVar_0_0: Tensor[(96, 64), float16], %FunctionVar_0_1: Tensor[(64, 64), float16], PartitionedFromPattern="nn.dense_", Composite="cutlass.dense") -> Tensor[(96, 64), float32] {
    nn.dense(%FunctionVar_0_0, %FunctionVar_0_1, units=None, out_dtype="float32") /* ty=Tensor[(96, 64), float32] */
  };
  %0(%cutlass_0_i0, %cutlass_0_i1) /* ty=Tensor[(96, 64), float32] */
}
```

## tune_cutlass_kernels

对于已经Partion过后的mod，profile并选择出最优的kernel实现。

```python
def tune_cutlass_kernels(
    mod, sm, use_3xtf32=True, profile_all=True, use_multiprocessing=False, tmp_dir="./tmp"
):
    '''
    Parameters
    ----------
    mod : IRModule
        The Relay module with cutlass partitions.
    sm : int
        An integer specifying the compute capability. 10 for iluvatar
    profile_all : bool
        Whether or not profile all candidate kernels, or stop profiling after
        the first applicable kernel is found.
    use_multiprocessing : bool
        Whether or not compile profiler executables for different kernels in parallel.
    tmp_dir : string, optional
        A temporary directory where intermediate compiled artifacts will be stored.
    Returns
    -------
    mod : IRModule
        The updated module annotated with cutlass profiling information.

    num_cutlass_partition : int
        The number of partitioned functions created for CUTLASS.
    '''
```

对于不同的op dense和batch_matmul使用了`CutlassGemmProfiler`, conv2d使用了`CutlassConv2DProfiler`。

```python
def tune_cutlass_kernels(mod, sm, use_3xtf32=True, split_k_slices=[1],
    profile_all_alignments=False, find_first_valid=False,
    use_multiprocessing=False, tmp_dir="./tmp",):
    gemm_profiler = CutlassGemmProfiler(sm, _get_cutlass_path(), tmp_dir)
    conv2d_profiler = CutlassConv2DProfiler(sm, _get_cutlass_path(), tmp_dir)
    num_cutlass_partition = 0
    for var in mod.get_global_vars():
        fun_name = var.name_hint
        if "cutlass" in fun_name:
            ...
            if "conv2d" in op_type:
                ...
                new_attrs.update(handle_conv2d(...))
            elif "batch_matmul" in op_type:
                new_attrs.update(handle_batch_matmul(...))
            elif "dense" in op_type:
                new_attrs.update(handle_dense(...))
            else:
                raise ValueError("%s unsupported composite" % op_type)

            new_attrs = tvm.ir.make_node("DictAttrs", **new_attrs)
            new_func = relay.Function(...)
            mod.update_func(var, new_func)

    return mod, num_cutlass_partition
```

`handle_conv2d`、`handle_batch_matmul`、`handle_dense`返回了`cutlass_op_def`：即cutlass的对应kernel参数下的最优代码实现，以及kernel name，然后用这个返回的Anotation重新生成Function。

其基础实现都是调用了之前定义的Profiler类的profile方法。以Gemm的实现为例CutlassGemmProfiler

generate_cu10_tensor_op_1688

enumerate_gemm_operators