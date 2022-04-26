# TVM CutLass

TVM中使用CutLass的TestCase在目录`tests/python/contrib/test_cutlass.py`

原有TVM中代码流程梳理如下:

```python
def get_dense_with_shape(data_shape, weight_shape, out_dtype="float16"):
    data = relay.var("data", shape=data_shape, dtype="float16")
    weight = relay.var("weight", shape=weight_shape, dtype="float16")
    return relay.nn.dense(data, weight, out_dtype=out_dtype)

def get_dense(M, N, K, out_dtype="float16"):
    return get_dense_with_shape((M, K), (N, K), out_dtype)

# 生成relay ir图
relay_ir = get_dense(M, N, K, out_dtype="float32")
```

在生成relay ir图之后，verify_dense。

verify_dense首先判断是否使用存在cutlass如何存在，通过获取global_func: relay.ext.cutlass这个Op是否存在判断是否编译的时候时候。

使用cutlass， 实际上就是曾了cutlass的codegen

```cmake
if(USE_CUDA AND USE_CUTLASS)
  file(GLOB CUTLASS_RELAY_CONTRIB_SRC src/relay/backend/contrib/cutlass/*.cc)
  list(APPEND COMPILER_SRCS ${CUTLASS_RELAY_CONTRIB_SRC})

  message(STATUS "Build with CUTLASS")
endif()
```

多编译了文件codegen.cc

```c++
TVM_REGISTER_GLOBAL("relay.ext.cutlass").set_body_typed(CutlassCompiler);
```



verify_dense由调用实现，使用之前的has_cutlass直接判断函数的TypedPackedFunc是否存在

```
def verify_dense(
    func, M, N, K, ref_target="cuda", sm=80, atol=1e-5, rtol=1e-5, run_benchmark=False
):
    if not has_cutlass():
        return
    mod = tvm.IRModule.from_expr(func)
    typ = relay.transform.InferType()(mod)["main"].body.checked_type
    out_dtype = typ.dtype
    np_data = np.random.uniform(-1, 1, (M, K)).astype("float16")
    np_weight = np.random.uniform(-1, 1, (N, K)).astype("float16")
    np_bias = np.random.uniform(-1, 1, (N,)).astype(out_dtype)

    params = {"weight": np_weight, "bias": np_bias}

    rt_mod_ref, dev = get_ref_rt_mod(mod, params, target=ref_target)
    rt_mod, dev, num_partition = profile_and_build(mod, params, sm)
    x = tvm.nd.array(np_data, device=dev)
    out = get_output(rt_mod, ["data"], [x])
    ref_out = get_output(rt_mod_ref, ["data"], [x])

    assert num_partition > 0
    np.testing.assert_allclose(out, ref_out, atol=atol, rtol=rtol)

    if run_benchmark:
        print("CUTLASS:", rt_mod.benchmark(dev, number=1, repeat=600))
        print("TVM with target %s:" % ref_target,
              rt_mod_ref.benchmark(dev, number=1, repeat=600))
```

