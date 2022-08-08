# QuantizeCalibrate

通俗的来说就是校准Pass

```python
def calibrate(dataset=None):
    """The calibrate procedure will try to calculate the content of
    dom_scale, nbit, clip_min, clip_max for every `simulated_quantize`
    operator.
    """

    def wrapped_func(mod, _):
        cfg = quantize.current_qconfig()
		# 对于输入存在kl_divergence、global_scale、percentile方式的校准
        if cfg.calibrate_mode == "kl_divergence":
            input_scale_func = _kl_scale(mod, dataset)
        elif cfg.calibrate_mode == "global_scale":
            input_scale_func = _global_scale
        elif cfg.calibrate_mode == "percentile":
            input_scale_func = _percentile_scale(mod, dataset)
        else:
            raise ValueError("Unknown calibrate mode {}".format(cfg.calibrate_mode))
		# 对数weight 存在max和power2 计算scale的值
        if cfg.weight_scale == "max":
            weight_scale_func = _max_scale
        elif cfg.weight_scale == "power2":
            weight_scale_func = _power2_scale
        else:
            raise ValueError("Unknown weight scale mode {}".format(cfg.weight_scale))

        return _set_params(mod, input_scale_func, weight_scale_func)

    return wrapped_func
```

对于global_scale无需进行计算全局使用统一的配置，percentile丢弃掉一些极端值，选择出一个max出来
```python
def _find_scale_by_percentile(arr, percentile=0.99999):
    assert isinstance(arr, np.ndarray)
    x = np.abs(arr)
    max_k = int(x.size * percentile)
    return np.partition(x, max_k)[max_k]
```

以上两种都比较基础，而下面这种则是使用kl_divergence的方法计算，其主要参考的实现是TensorRT GTC  [8-bit Inference with TensorRT](https://on-demand.gputechconf.com/gtc/2017/presentation/s7310-8-bit-inference-with-tensorrt.pdf)

```python
def _find_scale_by_kl(arr, quantized_dtype="int8", num_bins=8001, num_quantized_bins=255):
    """Given a tensor, find the optimal threshold for quantizing it.
    The reference distribution is `q`, and the candidate distribution is `p`.
    `q` is a truncated version of the original distribution.

    Ref:
    http://on-demand.gputechconf.com/gtc/2017/presentation/s7310-8-bit-inference-with-tensorrt.pdf
    """
    assert isinstance(arr, np.ndarray)
    min_val = np.min(arr)
    max_val = np.max(arr)
    thres = max(abs(min_val), abs(max_val))

    if min_val >= 0 and quantized_dtype in ["uint8"]:
        # We need to move negative bins to positive bins to fit uint8 range.
        num_quantized_bins = num_quantized_bins * 2 + 1

    def get_pointer(arr, ctypes_type):
        ptr = arr.ctypes.data_as(ctypes.POINTER(ctypes_type))
        return ctypes.cast(ptr, ctypes.c_void_p)

    hist, hist_edges = np.histogram(arr, bins=num_bins, range=(-thres, thres))
    hist_ptr = get_pointer(hist.astype(np.int32), ctypes.c_int)
    hist_edges_ptr = get_pointer(hist_edges, ctypes.c_float)

    return _quantize.FindScaleByKLMinimization(
        hist_ptr, hist_edges_ptr, num_bins, num_quantized_bins
    )
```

