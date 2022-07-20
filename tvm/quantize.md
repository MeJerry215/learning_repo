# Quantize

量化是一个经久不衰的问题，量化可以减小模型的大小，运行加载内存，减少数据传输，提高模型性能。以牺牲一定精度的前提下，获得性能的提升。

而在机器学习领域中，已经被验证量化对于网络损失的精度还是可以接受的。

虽然tvm自身带了量化，但是其官方态度比较倾向于使用外部的框架量化然后再导入进tvm。

本文只是相对于tvm自身量化做了什么进行一个简单的分析。

```python
def quantize(mod, params=None, dataset=None):
    # 量化前的通用优化
    mod = prerequisite_optimize(mod, params)
	# 模型进行量化
    calibrate_pass = tvm.transform.module_pass(
        calibrate(dataset), opt_level=1, name="QuantizeCalibrate"
    )
    quant_passes = [partition(), annotate(), calibrate_pass, tvm.relay.transform.InferType()]
    if not current_qconfig().do_simulation:
        quant_passes.append(realize())
    quant_passes.append(_transform.FoldConstant())
    quantize_seq = tvm.transform.Sequential(quant_passes)
    with tvm.transform.PassContext(
        opt_level=3, required_pass=["QuantizeAnnotate", "QuantizeCalibrate", "QuantizeRealize"]
    ):
        with quantize_context():
            mod = quantize_seq(mod)
	
    q_cfg = current_qconfig()
    ...

    return mod


def prerequisite_optimize(mod, params=None):
    optimize = tvm.transform.Sequential(
        [
            _transform.SimplifyInference(),
            _transform.FoldConstant(),
            _transform.FoldScaleAxis(),
            _transform.CanonicalizeOps(),
            _transform.FoldConstant(),
        ]
    )

    if params:
        mod["main"] = _bind_params(mod["main"], params)

    mod = optimize(mod)
    return mod
```

`prerequisite_optimize`会执行5个Pass， 其中`SimplifyInference`、`FoldConstant`、`FoldScaleAxis` 通常组合来用来消除图中的`batchnorm`op，`batch norm`在推理过程中直接表示为乘加操作。具体可以参考[What is FoldScaleAxis optimization](https://discuss.tvm.apache.org/t/what-is-foldscaleaxis-optimization/11913)

`CanonicalizeOps` 则是将`bias_add` op 转换为 `expand_dims` + `add` op的组合，将表达式一般化。

`FoldConstant`： 顾名思义就是将常量折叠，这是一种通用的优化，提前计算掉图中能够计算的op，从而减少推理时的运算量，在tvm中，常量折叠使用的是lower cpu target进行计算。

在上述过程中只是进行一些通用的优化，来使得图更加的简洁。

在这之后就是量化的核心Pass：

首先第一个Pass就是`QuantizePartition`, 

