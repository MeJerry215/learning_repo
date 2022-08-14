# MergeComposite

放一段注释原话，merge composite pass就是将多个op 融合为一个function同时打上标签，这样做的目的是啥呢，主要是为了在后续的流程中能够lower到对应相应的codegen中，调用或者生成代码。

```python
"""
The merge composite pass is designed to merge multiple relay operators, that
match a given pattern, and combine them into a single relay function.

For example suppose we have the graph:

    conv2d
      |       (merge composite pass)
   bias_add            ====>           conv2d_bias_relu
      |            (our target)
     relu

Our Relay IR before the pass:
    fn (%data: Tensor[(1, 512, 28, 28), float32], %kernel: Tensor[(256, 512, 1, 1), float32],
            %bias: Tensor[(256), float32]) -> Tensor[(1, 256, 28, 28), float32] {
        %0 = nn.conv2d(%data, %kernel, kernel_size=[1, 1]) /* ty=Tensor[(1, 256, 28, 28), float32] */;
        %1 = nn.bias_add(%0, %bias) /* ty=Tensor[(1, 256, 28, 28), float32] */;
        nn.relu(%1) /* ty=Tensor[(1, 256, 28, 28), float32] */
    }

Our Relay IR after the pass:
    fn (%data: Tensor[(1, 512, 28, 28), float32], %kernel: Tensor[(256, 512, 1, 1), float32],
            %bias: Tensor[(256), float32]) -> Tensor[(1, 256, 28, 28), float32] {
      %2 = fn (%x: Tensor[(1, 512, 28, 28), float32], %y: Tensor[(256, 512, 1, 1), float32],
            %z: Tensor[(256), float32], Primitive=1, Composite="conv2d_bias_relu") ->
            Tensor[(1, 256, 28, 28), float32] {
        %0 = nn.conv2d(%x, %y, kernel_size=[1, 1]) /* ty=Tensor[(1, 256, 28, 28), float32] */;
        %1 = nn.bias_add(%0, %z) /* ty=Tensor[(1, 256, 28, 28), float32] */;
        nn.relu(%1) /* ty=Tensor[(1, 256, 28, 28), float32] */
      };
      %2(%data, %kernel, %bias) /* ty=Tensor[(1, 256, 28, 28), float32] */
    }

As you can see in the second relay example, the pattern we specified has been wrapped
in a function. The function is then called, producing the same result as the first relay
example.

One convenient use for this pass is to offload multiple operators to a single external
codegen function.
"""
```

自己运行一个简单的add_relu的ir如下所示

```c++
fn (%a: Tensor[(10, 10), float32], %b: Tensor[(10, 10), float32]) {
  %0 = add(%a, %b);
  nn.relu(%0)
} 

fn (%a: Tensor[(10, 10), float32], %b: Tensor[(10, 10), float32]) -> Tensor[(10, 10), float32] {
  %1 = fn (%in_1: Tensor[(10, 10), float32], %in_2: Tensor[(10, 10), float32], Composite="add_relu", PartitionedFromPattern="add_nn.relu_") -> Tensor[(10, 10), float32] {
    %0 = add(%in_1, %in_2) /* ty=Tensor[(10, 10), float32] */;
    nn.relu(%0) /* ty=Tensor[(10, 10), float32] */
  };
  %1(%a, %b) /* ty=Tensor[(10, 10), float32] */
}
```

MergeComposite 接受一个入参pattern_table，将满足pattern_table中的op打上对应的pattern名

```python
pattern_table = [
   ("add_relu", make_add_relu_pattern()),
   ("conv_bias_relu", make_conv_bias_relu_pattern(), _check_type_true),
]
```

