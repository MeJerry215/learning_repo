# QuantizePartition

`ForwardRewriter`继承自`MixedModeMutator`。稍微回顾一下`MixedModeMutator`，通过`ExpandDataflow`将节点压栈的的方式对图进行DFS遍历，Rewrite方法，对于返回了一份对于该节点的拷贝。

这里先将一个叫做`TempRealizer`的类， 这个类的主要作用就是当访问节点的时候，如果节点是`TempExprNode`类型的，那么就调用其`Realize`函数。

常见的比如`QPartitionExprNode`，其Realize函数就是在节点后面插入`CastHint`和`StopFusion`节点。

```python
def partition():
    """Partition graph into small low-precision sections by `cast_hint` and
    `stop_fusion`.

    Returns
    -------
    ret: tvm.transform.Pass
        The registered pass for VTA rewrite.
    """
    return _quantize.QuantizePartition()
```

查看其描述就是将图切分为低精度的section，主要靠插入是cast_hint和stop_fusion。插入的cast是 转换成int8的cast，后面插入stop_fusion主要是为了阻止后续的融合产生非预期的效果，我们更希望的是网络层间传输的数据是int8的数据。

```c++
Expr QPartitionExprNode::Realize() const {
  // insert cast hint and stop fusion
  const QConfig& cfg = QConfig::Current();
  Expr ret = CastHint(this->expr, cfg->dtype_input);
  return StopFusion(ret);
}
```

然后`ForwardRewriter`就是一个前向重写遍历器，将访问到符合规则的节点重写， 其内部存在一个`rewrite_map_`存储节点和重写函数的关系map，`rewrite_func_`针对单一规则的重写函数。`fcontext_`上下文，`fmulti_ref_trigger_`多处引用到同一个节点的重写触发函数。`ref_counter_`引用计数。

```c++
class ForwardRewriter : private MixedModeMutator {
 private:
  // The rewrite rule.
  const OpAttrMap<FForwardRewrite>* rewrite_map_{nullptr};
  const FForwardRewrite* rewrite_func_{nullptr};
  // The context.const
  std::function<ObjectRef(const Call&)> fcontext_{nullptr};
  // The multiple reference trigger
  std::function<Expr(const Expr&)> fmulti_ref_trigger_{nullptr};
  // Internal ref counter
  std::unordered_map<const Object*, size_t> ref_counter_;
  // internal realizer
  TempRealizer realizer_;
}
```

由于其遍历流程和`MixedModeMutator`一直，所以只重写了`Rewriter_`函数。对于`TupleGetItemNode`和`TupleNode`主要是看有没有multiref函数对齐重写，然后将重新写后的函数原样返回。其主要流程在于对于`CallNode`的`Rewrite_`

```c++
Expr Rewrite_(const CallNode *call_node, const Expr &post) final {
    const Call &ref_call = GetRef<Call>(call_node);
    PackedFunc frewrite;
    // 如果存在rewrite func则使用，否则根据call的op选择rewrite func
    if (rewrite_func_) {
        frewrite = *rewrite_func_;
    }
    else {
        ICHECK(rewrite_map_);
        frewrite = rewrite_map_->get(call_node->op, nullptr);
    }
    const auto *post_node = post.as<CallNode>();
    auto new_op = post_node->op;
    bool unchanged = call_node->op.same_as(new_op);

    Array<Expr> call_args;
    for (size_t i = 0; i < call_node->args.size(); ++i) {
        Expr new_arg = this->GetTempExpr(call_node->args[i], post_node->args[i]);
        if (frewrite == nullptr)
        {
            // 对入参调用Realize
            new_arg = realizer_.Realize(new_arg);
        }
        unchanged &= new_arg.same_as(call_node->args[i]);
        // 将新的Realize之后的入参压入栈
        call_args.push_back(new_arg);
    }
    // try to rewrite.
    if (frewrite != nullptr) {
        // rewrite为一个新的expr
        Expr res = frewrite(ref_call, call_args,
                            fcontext_ != nullptr ? fcontext_(ref_call) : ObjectRef(nullptr));
        if (res.defined())
            return res;
        // 重新对call args调用Realize，然后设置回去, 这里的Realize将所有之前的TempExpr全部Realize掉
        for (size_t i = 0; i < call_args.size(); ++i)
        {
            Expr arg = call_args[i];
            Expr new_arg = realizer_.Realize(arg);
            if (!arg.same_as(new_arg))
            {
                call_args.Set(i, new_arg);
                unchanged = false;
            }
        }
    }
    if (unchanged)
        return ref_call;
    return Call(new_op, call_args, call_node->attrs, call_node->type_args, call_node->span);
}
```

总体上来说`ForwardRewrite`主要是主要是调用Realize替换了op的实现(一般为插入了一些op)，然后调用Rewrite函数重写。

其实我这里想讲的是另外一个Pass， `QuantizePartition` Pass底层使用了`ForwardRewrite`。对注册了`FQPartitionRewrite`属性的节点调用rewrite函数。

```c++
Pass QuantizePartition() {
  runtime::TypedPackedFunc<Function(Function, IRModule, PassContext)> pass_func =
      [=](Function f, IRModule m, PassContext pc) {
        auto ret = Downcast<Function>(ForwardRewrite(f, "FQPartitionRewrite", nullptr, nullptr));
        return ret;
      };
  return CreateFunctionPass(pass_func, 1, "QuantizePartition", {});
}
```

在`python/tvm/relay/quantize/_partition.py`中定义了属性注册函数

```python
def register_partition_function(op_name, frewrite=None, level=10):
    return tvm.ir.register_op_attr(op_name, "FQPartitionRewrite", frewrite, level)
```

在_partition中主要注册了conv2d、add、multiply、clip、nn.relu、nn.max_pool2d、nn.global_avg_pool2d的partion函数。





```c++
/*
conv2d   
         data  weight                  data(QPartitionExpr)	 weight
			\     /								\               /
			  conv                             cast  		   /
               |                                  \           /
          QPartitionExpr                      stop_fusion    /
                                                    \       /
                                                      conv
                                                        |
                                                  QPartitionExpr
QPartitionExpr data 调用realize 产生cast和stop_fusion， 同时去掉QPartionExpr
*/

/*
identity:(clip, relu, nn.max_pool2d)
		data(QPartitionExpr)
        		|
			 identity
			 	|
			QPartitionExpr
*/

/*
multiply
		  data(QPartitionExpr)          weight
		  			\                    /
				    cast                /
				       \               /
				       stop_fusion    /
				          \          /
				            multiply
				               |
				         QPartitionExpr
*/


/*
global_avg_pool2d
					data(QPartitionExpr)                 data
							|							   |
						   cast                       QPartitionExpr
						    |                              |
					   stop_fusion                       cast
					        |                              |
				   global_avg_pool2d                   stop_fusion
				                                           |
				                                     global_avg_pool2d
总是在global_avg_pool2d前插入cast, stop_fusion
*/

/*
add
	  data(QPartitionExpr)  weight(QPartitionExpr)
			|						|
		   cast					   cast
		    |						|
		stop_fusion             stop_fusion
			     \			    /
					   add
					    |
					QPartitionExpr
					
					
	data		weight(QPartitionExpr)
	  |					|
	  \                cast
	    \				|
	  	  \	      stop_fusion
	       \       /
	          add
	          
	 data(QPartitionExpr)   weight(const)
	 		   |			/
			      add
			       |
			    QPartitionExpr
			    
			    
    data(QPartitionExpr)	weight(not const)
    		|				/
    	   cast           /
    		|			/
    	 stop_fusion  /
    	    |       /
    	       add
*/			


```

在这个图里面，卷积前面调用了调用了调用了realize

这里说一下为什么会产生这样的图

```c++
  %0 = nn.pad(%data, 0 /* ty=int32 */, pad_width=[[0, 0], [0, 0], [0, 0], [0, 1]]) /* ty=Tensor[(32, 224, 224, 4), float32] */;
  %1 = nn.conv2d(%0, meta[relay.Constant][0] /* ty=Tensor[(7, 7, 4, 64), float32] */, strides=[2, 2], padding=[3, 3, 3, 3], channels=64, kernel_size=[7, 7], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %2 = add(%1, meta[relay.Constant][1] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %3 = nn.relu(%2) /* resnet_v1_50/conv1/Relu */ /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %4 = nn.max_pool2d(%3, pool_size=[3, 3], strides=[2, 2], padding=[0, 0, 1, 1], layout="NHWC") /* resnet_v1_50/pool1/MaxPool */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %5 = nn.conv2d(%4, meta[relay.Constant][2] /* ty=Tensor[(1, 1, 64, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 256), float32] */;
```

这里考虑的是`QPartitionExpr`在层之间的传递过程

首先对nn.conv2d产生调用Rewrite方法，产生了一个QPartitionExpr包装了conv2d

```c++
  %0 = nn.pad(%data, 0 /* ty=int32 */, pad_width=[[0, 0], [0, 0], [0, 0], [0, 1]]) /* ty=Tensor[(32, 224, 224, 4), float32] */;
  %1 = nn.conv2d(%0, meta[relay.Constant][0] /* ty=Tensor[(7, 7, 4, 64), float32] */, strides=[2, 2], padding=[3, 3, 3, 3], channels=64, kernel_size=[7, 7], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 112, 112, 64), float32] */;
%6 = QPartitionExpr(%1)
  %2 = add(%6, meta[relay.Constant][1] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %3 = nn.relu(%2) /* resnet_v1_50/conv1/Relu */ /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %4 = nn.max_pool2d(%3, pool_size=[3, 3], strides=[2, 2], padding=[0, 0, 1, 1], layout="NHWC") /* resnet_v1_50/pool1/MaxPool */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %5 = nn.conv2d(%4, meta[relay.Constant][2] /* ty=Tensor[(1, 1, 64, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 256), float32] */;
```

然后add的时候，data是QPartitionExpr， 另外一路是const，则继续向下传递QPartitionExpr

```c++
  %0 = nn.pad(%data, 0 /* ty=int32 */, pad_width=[[0, 0], [0, 0], [0, 0], [0, 1]]) /* ty=Tensor[(32, 224, 224, 4), float32] */;
  %1 = nn.conv2d(%0, meta[relay.Constant][0] /* ty=Tensor[(7, 7, 4, 64), float32] */, strides=[2, 2], padding=[3, 3, 3, 3], channels=64, kernel_size=[7, 7], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %2 = add(%1, meta[relay.Constant][1] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 112, 112, 64), float32] */;
%6 = QPartitionExpr(%2)
  %3 = nn.relu(%6) /* resnet_v1_50/conv1/Relu */ /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %4 = nn.max_pool2d(%3, pool_size=[3, 3], strides=[2, 2], padding=[0, 0, 1, 1], layout="NHWC") /* resnet_v1_50/pool1/MaxPool */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %5 = nn.conv2d(%4, meta[relay.Constant][2] /* ty=Tensor[(1, 1, 64, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 256), float32] */;
```

继续向下遇到relu，relu和maxpool_2d属于Identity，只负责向下传递QPartitionExpr

```c++
  %0 = nn.pad(%data, 0 /* ty=int32 */, pad_width=[[0, 0], [0, 0], [0, 0], [0, 1]]) /* ty=Tensor[(32, 224, 224, 4), float32] */;
  %1 = nn.conv2d(%0, meta[relay.Constant][0] /* ty=Tensor[(7, 7, 4, 64), float32] */, strides=[2, 2], padding=[3, 3, 3, 3], channels=64, kernel_size=[7, 7], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %2 = add(%1, meta[relay.Constant][1] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %3 = nn.relu(%2) /* resnet_v1_50/conv1/Relu */ /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %4 = nn.max_pool2d(%3, pool_size=[3, 3], strides=[2, 2], padding=[0, 0, 1, 1], layout="NHWC") /* resnet_v1_50/pool1/MaxPool */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
%6 = QPartitionExpr(%4)
  %5 = nn.conv2d(%6, meta[relay.Constant][2] /* ty=Tensor[(1, 1, 64, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 256), float32] */;
```

直到遇到conv2d，这个时候插入cast和stop_fusion。为什么要在这里插入? 我们对conv2d进行了量化，同时存在tvm自身的FuseOps的存在，预期场景下我conv2d能够和后面的op进行融合一直到输出int8。所以我就需要在某个地方标记停止融合，再融合会导致出问题，融合op之间的数据传输不是int8了。

![image-20220804184903660](D:\Repo\learning_repo\tvm\QuantizePartition.assets\image-20220804184903660.png)









## Before Partition

```c++
def @main /* id=396465712 */(%data: Tensor[(32, 224, 224, 3), float32]) -> Tensor[(32, 1, 1, 1000), float32] {
  %0 = nn.pad(%data, 0 /* ty=int32 */, pad_width=[[0, 0], [0, 0], [0, 0], [0, 1]]) /* ty=Tensor[(32, 224, 224, 4), float32] */;
  %1 = nn.conv2d(%0, meta[relay.Constant][0] /* ty=Tensor[(7, 7, 4, 64), float32] */, strides=[2, 2], padding=[3, 3, 3, 3], channels=64, kernel_size=[7, 7], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %2 = add(%1, meta[relay.Constant][1] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %3 = nn.relu(%2) /* resnet_v1_50/conv1/Relu */ /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %4 = nn.max_pool2d(%3, pool_size=[3, 3], strides=[2, 2], padding=[0, 0, 1, 1], layout="NHWC") /* resnet_v1_50/pool1/MaxPool */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %5 = nn.conv2d(%4, meta[relay.Constant][2] /* ty=Tensor[(1, 1, 64, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %6 = nn.conv2d(%4, meta[relay.Constant][4] /* ty=Tensor[(1, 1, 64, 64), float32] */, padding=[0, 0, 0, 0], channels=64, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %7 = add(%6, meta[relay.Constant][5] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %8 = nn.relu(%7) /* resnet_v1_50/block1/unit_1/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %9 = nn.conv2d(%8, meta[relay.Constant][6] /* ty=Tensor[(3, 3, 64, 64), float32] */, padding=[1, 1, 1, 1], channels=64, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %10 = add(%9, meta[relay.Constant][7] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %11 = nn.relu(%10) /* resnet_v1_50/block1/unit_1/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %12 = nn.conv2d(%11, meta[relay.Constant][8] /* ty=Tensor[(1, 1, 64, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %13 = add(%5, meta[relay.Constant][3] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %14 = add(%12, meta[relay.Constant][9] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %15 = add(%13, %14) /* resnet_v1_50/block1/unit_1/bottleneck_v1/add */ /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %16 = nn.relu(%15) /* resnet_v1_50/block1/unit_1/bottleneck_v1/Relu */ /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %17 = nn.conv2d(%16, meta[relay.Constant][10] /* ty=Tensor[(1, 1, 256, 64), float32] */, padding=[0, 0, 0, 0], channels=64, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %18 = add(%17, meta[relay.Constant][11] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %19 = nn.relu(%18) /* resnet_v1_50/block1/unit_2/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %20 = nn.conv2d(%19, meta[relay.Constant][12] /* ty=Tensor[(3, 3, 64, 64), float32] */, padding=[1, 1, 1, 1], channels=64, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %21 = add(%20, meta[relay.Constant][13] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %22 = nn.relu(%21) /* resnet_v1_50/block1/unit_2/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %23 = nn.conv2d(%22, meta[relay.Constant][14] /* ty=Tensor[(1, 1, 64, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %24 = add(%23, meta[relay.Constant][15] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %25 = add(%16, %24) /* resnet_v1_50/block1/unit_2/bottleneck_v1/add */ /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %26 = nn.relu(%25) /* resnet_v1_50/block1/unit_2/bottleneck_v1/Relu */ /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %27 = nn.conv2d(%26, meta[relay.Constant][16] /* ty=Tensor[(1, 1, 256, 64), float32] */, padding=[0, 0, 0, 0], channels=64, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %28 = add(%27, meta[relay.Constant][17] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %29 = nn.relu(%28) /* resnet_v1_50/block1/unit_3/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %30 = nn.conv2d(%29, meta[relay.Constant][18] /* ty=Tensor[(3, 3, 64, 64), float32] */, strides=[2, 2], padding=[1, 1, 1, 1], channels=64, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 64), float32] */;
  %31 = add(%30, meta[relay.Constant][19] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 28, 28, 64), float32] */;
  %32 = nn.relu(%31) /* resnet_v1_50/block1/unit_3/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 28, 28, 64), float32] */;
  %33 = nn.conv2d(%32, meta[relay.Constant][20] /* ty=Tensor[(1, 1, 64, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %34 = nn.max_pool2d(%26, pool_size=[1, 1], strides=[2, 2], padding=[0, 0, 0, 0], layout="NHWC") /* resnet_v1_50/block1/unit_3/bottleneck_v1/shortcut/MaxPool */ /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %35 = add(%33, meta[relay.Constant][21] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %36 = add(%34, %35) /* resnet_v1_50/block1/unit_3/bottleneck_v1/add */ /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %37 = nn.relu(%36) /* resnet_v1_50/block1/unit_3/bottleneck_v1/Relu */ /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %38 = nn.conv2d(%37, meta[relay.Constant][22] /* ty=Tensor[(1, 1, 256, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %39 = nn.conv2d(%37, meta[relay.Constant][24] /* ty=Tensor[(1, 1, 256, 128), float32] */, padding=[0, 0, 0, 0], channels=128, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %40 = add(%39, meta[relay.Constant][25] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %41 = nn.relu(%40) /* resnet_v1_50/block2/unit_1/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %42 = nn.conv2d(%41, meta[relay.Constant][26] /* ty=Tensor[(3, 3, 128, 128), float32] */, padding=[1, 1, 1, 1], channels=128, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %43 = add(%42, meta[relay.Constant][27] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %44 = nn.relu(%43) /* resnet_v1_50/block2/unit_1/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %45 = nn.conv2d(%44, meta[relay.Constant][28] /* ty=Tensor[(1, 1, 128, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %46 = add(%38, meta[relay.Constant][23] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %47 = add(%45, meta[relay.Constant][29] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %48 = add(%46, %47) /* resnet_v1_50/block2/unit_1/bottleneck_v1/add */ /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %49 = nn.relu(%48) /* resnet_v1_50/block2/unit_1/bottleneck_v1/Relu */ /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %50 = nn.conv2d(%49, meta[relay.Constant][30] /* ty=Tensor[(1, 1, 512, 128), float32] */, padding=[0, 0, 0, 0], channels=128, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %51 = add(%50, meta[relay.Constant][31] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %52 = nn.relu(%51) /* resnet_v1_50/block2/unit_2/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %53 = nn.conv2d(%52, meta[relay.Constant][32] /* ty=Tensor[(3, 3, 128, 128), float32] */, padding=[1, 1, 1, 1], channels=128, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %54 = add(%53, meta[relay.Constant][33] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %55 = nn.relu(%54) /* resnet_v1_50/block2/unit_2/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %56 = nn.conv2d(%55, meta[relay.Constant][34] /* ty=Tensor[(1, 1, 128, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %57 = add(%56, meta[relay.Constant][35] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %58 = add(%49, %57) /* resnet_v1_50/block2/unit_2/bottleneck_v1/add */ /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %59 = nn.relu(%58) /* resnet_v1_50/block2/unit_2/bottleneck_v1/Relu */ /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %60 = nn.conv2d(%59, meta[relay.Constant][36] /* ty=Tensor[(1, 1, 512, 128), float32] */, padding=[0, 0, 0, 0], channels=128, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %61 = add(%60, meta[relay.Constant][37] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %62 = nn.relu(%61) /* resnet_v1_50/block2/unit_3/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %63 = nn.conv2d(%62, meta[relay.Constant][38] /* ty=Tensor[(3, 3, 128, 128), float32] */, padding=[1, 1, 1, 1], channels=128, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %64 = add(%63, meta[relay.Constant][39] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %65 = nn.relu(%64) /* resnet_v1_50/block2/unit_3/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %66 = nn.conv2d(%65, meta[relay.Constant][40] /* ty=Tensor[(1, 1, 128, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %67 = add(%66, meta[relay.Constant][41] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %68 = add(%59, %67) /* resnet_v1_50/block2/unit_3/bottleneck_v1/add */ /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %69 = nn.relu(%68) /* resnet_v1_50/block2/unit_3/bottleneck_v1/Relu */ /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %70 = nn.conv2d(%69, meta[relay.Constant][42] /* ty=Tensor[(1, 1, 512, 128), float32] */, padding=[0, 0, 0, 0], channels=128, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %71 = add(%70, meta[relay.Constant][43] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %72 = nn.relu(%71) /* resnet_v1_50/block2/unit_4/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %73 = nn.conv2d(%72, meta[relay.Constant][44] /* ty=Tensor[(3, 3, 128, 128), float32] */, strides=[2, 2], padding=[1, 1, 1, 1], channels=128, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 128), float32] */;
  %74 = add(%73, meta[relay.Constant][45] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 14, 14, 128), float32] */;
  %75 = nn.relu(%74) /* resnet_v1_50/block2/unit_4/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 14, 14, 128), float32] */;
  %76 = nn.conv2d(%75, meta[relay.Constant][46] /* ty=Tensor[(1, 1, 128, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %77 = nn.max_pool2d(%69, pool_size=[1, 1], strides=[2, 2], padding=[0, 0, 0, 0], layout="NHWC") /* resnet_v1_50/block2/unit_4/bottleneck_v1/shortcut/MaxPool */ /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %78 = add(%76, meta[relay.Constant][47] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %79 = add(%77, %78) /* resnet_v1_50/block2/unit_4/bottleneck_v1/add */ /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %80 = nn.relu(%79) /* resnet_v1_50/block2/unit_4/bottleneck_v1/Relu */ /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %81 = nn.conv2d(%80, meta[relay.Constant][48] /* ty=Tensor[(1, 1, 512, 1024), float32] */, padding=[0, 0, 0, 0], channels=1024, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %82 = nn.conv2d(%80, meta[relay.Constant][50] /* ty=Tensor[(1, 1, 512, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %83 = add(%82, meta[relay.Constant][51] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %84 = nn.relu(%83) /* resnet_v1_50/block3/unit_1/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %85 = nn.conv2d(%84, meta[relay.Constant][52] /* ty=Tensor[(3, 3, 256, 256), float32] */, padding=[1, 1, 1, 1], channels=256, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %86 = add(%85, meta[relay.Constant][53] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %87 = nn.relu(%86) /* resnet_v1_50/block3/unit_1/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %88 = nn.conv2d(%87, meta[relay.Constant][54] /* ty=Tensor[(1, 1, 256, 1024), float32] */, padding=[0, 0, 0, 0], channels=1024, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %89 = add(%81, meta[relay.Constant][49] /* ty=Tensor[(1024), float32] */) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %90 = add(%88, meta[relay.Constant][55] /* ty=Tensor[(1024), float32] */) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %91 = add(%89, %90) /* resnet_v1_50/block3/unit_1/bottleneck_v1/add */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %92 = nn.relu(%91) /* resnet_v1_50/block3/unit_1/bottleneck_v1/Relu */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %93 = nn.conv2d(%92, meta[relay.Constant][56] /* ty=Tensor[(1, 1, 1024, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %94 = add(%93, meta[relay.Constant][57] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %95 = nn.relu(%94) /* resnet_v1_50/block3/unit_2/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %96 = nn.conv2d(%95, meta[relay.Constant][58] /* ty=Tensor[(3, 3, 256, 256), float32] */, padding=[1, 1, 1, 1], channels=256, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %97 = add(%96, meta[relay.Constant][59] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %98 = nn.relu(%97) /* resnet_v1_50/block3/unit_2/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %99 = nn.conv2d(%98, meta[relay.Constant][60] /* ty=Tensor[(1, 1, 256, 1024), float32] */, padding=[0, 0, 0, 0], channels=1024, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %100 = add(%99, meta[relay.Constant][61] /* ty=Tensor[(1024), float32] */) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %101 = add(%92, %100) /* resnet_v1_50/block3/unit_2/bottleneck_v1/add */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %102 = nn.relu(%101) /* resnet_v1_50/block3/unit_2/bottleneck_v1/Relu */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %103 = nn.conv2d(%102, meta[relay.Constant][62] /* ty=Tensor[(1, 1, 1024, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %104 = add(%103, meta[relay.Constant][63] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %105 = nn.relu(%104) /* resnet_v1_50/block3/unit_3/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %106 = nn.conv2d(%105, meta[relay.Constant][64] /* ty=Tensor[(3, 3, 256, 256), float32] */, padding=[1, 1, 1, 1], channels=256, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %107 = add(%106, meta[relay.Constant][65] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %108 = nn.relu(%107) /* resnet_v1_50/block3/unit_3/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %109 = nn.conv2d(%108, meta[relay.Constant][66] /* ty=Tensor[(1, 1, 256, 1024), float32] */, padding=[0, 0, 0, 0], channels=1024, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %110 = add(%109, meta[relay.Constant][67] /* ty=Tensor[(1024), float32] */) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %111 = add(%102, %110) /* resnet_v1_50/block3/unit_3/bottleneck_v1/add */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %112 = nn.relu(%111) /* resnet_v1_50/block3/unit_3/bottleneck_v1/Relu */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %113 = nn.conv2d(%112, meta[relay.Constant][68] /* ty=Tensor[(1, 1, 1024, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %114 = add(%113, meta[relay.Constant][69] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %115 = nn.relu(%114) /* resnet_v1_50/block3/unit_4/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %116 = nn.conv2d(%115, meta[relay.Constant][70] /* ty=Tensor[(3, 3, 256, 256), float32] */, padding=[1, 1, 1, 1], channels=256, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %117 = add(%116, meta[relay.Constant][71] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %118 = nn.relu(%117) /* resnet_v1_50/block3/unit_4/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %119 = nn.conv2d(%118, meta[relay.Constant][72] /* ty=Tensor[(1, 1, 256, 1024), float32] */, padding=[0, 0, 0, 0], channels=1024, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %120 = add(%119, meta[relay.Constant][73] /* ty=Tensor[(1024), float32] */) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %121 = add(%112, %120) /* resnet_v1_50/block3/unit_4/bottleneck_v1/add */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %122 = nn.relu(%121) /* resnet_v1_50/block3/unit_4/bottleneck_v1/Relu */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %123 = nn.conv2d(%122, meta[relay.Constant][74] /* ty=Tensor[(1, 1, 1024, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %124 = add(%123, meta[relay.Constant][75] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %125 = nn.relu(%124) /* resnet_v1_50/block3/unit_5/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %126 = nn.conv2d(%125, meta[relay.Constant][76] /* ty=Tensor[(3, 3, 256, 256), float32] */, padding=[1, 1, 1, 1], channels=256, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %127 = add(%126, meta[relay.Constant][77] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %128 = nn.relu(%127) /* resnet_v1_50/block3/unit_5/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %129 = nn.conv2d(%128, meta[relay.Constant][78] /* ty=Tensor[(1, 1, 256, 1024), float32] */, padding=[0, 0, 0, 0], channels=1024, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %130 = add(%129, meta[relay.Constant][79] /* ty=Tensor[(1024), float32] */) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %131 = add(%122, %130) /* resnet_v1_50/block3/unit_5/bottleneck_v1/add */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %132 = nn.relu(%131) /* resnet_v1_50/block3/unit_5/bottleneck_v1/Relu */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %133 = nn.conv2d(%132, meta[relay.Constant][80] /* ty=Tensor[(1, 1, 1024, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %134 = add(%133, meta[relay.Constant][81] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %135 = nn.relu(%134) /* resnet_v1_50/block3/unit_6/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %136 = nn.conv2d(%135, meta[relay.Constant][82] /* ty=Tensor[(3, 3, 256, 256), float32] */, strides=[2, 2], padding=[1, 1, 1, 1], channels=256, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 256), float32] */;
  %137 = add(%136, meta[relay.Constant][83] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 7, 7, 256), float32] */;
  %138 = nn.relu(%137) /* resnet_v1_50/block3/unit_6/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 7, 7, 256), float32] */;
  %139 = nn.conv2d(%138, meta[relay.Constant][84] /* ty=Tensor[(1, 1, 256, 1024), float32] */, padding=[0, 0, 0, 0], channels=1024, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %140 = nn.max_pool2d(%132, pool_size=[1, 1], strides=[2, 2], padding=[0, 0, 0, 0], layout="NHWC") /* resnet_v1_50/block3/unit_6/bottleneck_v1/shortcut/MaxPool */ /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %141 = add(%139, meta[relay.Constant][85] /* ty=Tensor[(1024), float32] */) /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %142 = add(%140, %141) /* resnet_v1_50/block3/unit_6/bottleneck_v1/add */ /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %143 = nn.relu(%142) /* resnet_v1_50/block3/unit_6/bottleneck_v1/Relu */ /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %144 = nn.conv2d(%143, meta[relay.Constant][86] /* ty=Tensor[(1, 1, 1024, 2048), float32] */, padding=[0, 0, 0, 0], channels=2048, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %145 = nn.conv2d(%143, meta[relay.Constant][88] /* ty=Tensor[(1, 1, 1024, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %146 = add(%145, meta[relay.Constant][89] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %147 = nn.relu(%146) /* resnet_v1_50/block4/unit_1/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %148 = nn.conv2d(%147, meta[relay.Constant][90] /* ty=Tensor[(3, 3, 512, 512), float32] */, padding=[1, 1, 1, 1], channels=512, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %149 = add(%148, meta[relay.Constant][91] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %150 = nn.relu(%149) /* resnet_v1_50/block4/unit_1/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %151 = nn.conv2d(%150, meta[relay.Constant][92] /* ty=Tensor[(1, 1, 512, 2048), float32] */, padding=[0, 0, 0, 0], channels=2048, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %152 = add(%144, meta[relay.Constant][87] /* ty=Tensor[(2048), float32] */) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %153 = add(%151, meta[relay.Constant][93] /* ty=Tensor[(2048), float32] */) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %154 = add(%152, %153) /* resnet_v1_50/block4/unit_1/bottleneck_v1/add */ /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %155 = nn.relu(%154) /* resnet_v1_50/block4/unit_1/bottleneck_v1/Relu */ /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %156 = nn.conv2d(%155, meta[relay.Constant][94] /* ty=Tensor[(1, 1, 2048, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %157 = add(%156, meta[relay.Constant][95] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %158 = nn.relu(%157) /* resnet_v1_50/block4/unit_2/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %159 = nn.conv2d(%158, meta[relay.Constant][96] /* ty=Tensor[(3, 3, 512, 512), float32] */, padding=[1, 1, 1, 1], channels=512, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %160 = add(%159, meta[relay.Constant][97] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %161 = nn.relu(%160) /* resnet_v1_50/block4/unit_2/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %162 = nn.conv2d(%161, meta[relay.Constant][98] /* ty=Tensor[(1, 1, 512, 2048), float32] */, padding=[0, 0, 0, 0], channels=2048, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %163 = add(%162, meta[relay.Constant][99] /* ty=Tensor[(2048), float32] */) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %164 = add(%155, %163) /* resnet_v1_50/block4/unit_2/bottleneck_v1/add */ /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %165 = nn.relu(%164) /* resnet_v1_50/block4/unit_2/bottleneck_v1/Relu */ /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %166 = nn.conv2d(%165, meta[relay.Constant][100] /* ty=Tensor[(1, 1, 2048, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %167 = add(%166, meta[relay.Constant][101] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %168 = nn.relu(%167) /* resnet_v1_50/block4/unit_3/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %169 = nn.conv2d(%168, meta[relay.Constant][102] /* ty=Tensor[(3, 3, 512, 512), float32] */, padding=[1, 1, 1, 1], channels=512, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %170 = add(%169, meta[relay.Constant][103] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %171 = nn.relu(%170) /* resnet_v1_50/block4/unit_3/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %172 = nn.conv2d(%171, meta[relay.Constant][104] /* ty=Tensor[(1, 1, 512, 2048), float32] */, padding=[0, 0, 0, 0], channels=2048, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %173 = add(%172, meta[relay.Constant][105] /* ty=Tensor[(2048), float32] */) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %174 = add(%165, %173) /* resnet_v1_50/block4/unit_3/bottleneck_v1/add */ /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %175 = nn.relu(%174) /* resnet_v1_50/block4/unit_3/bottleneck_v1/Relu */ /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %176 = mean(%175, axis=[1, 2], keepdims=True) /* resnet_v1_50/pool5 */ /* ty=Tensor[(32, 1, 1, 2048), float32] */;
  %177 = nn.conv2d(%176, meta[relay.Constant][106] /* ty=Tensor[(1, 1, 2048, 1000), float32] */, padding=[0, 0, 0, 0], channels=1000, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* resnet_v1_50/logits/Conv2D */ /* ty=Tensor[(32, 1, 1, 1000), float32] */;
  add(%177, meta[relay.Constant][107] /* ty=Tensor[(1000), float32] */) /* resnet_v1_50/logits/BiasAdd */ /* ty=Tensor[(32, 1, 1, 1000), float32] */
}
```

## After Partition

```c++

def @main /* id=396465712 */(%data: Tensor[(32, 224, 224, 3), float32]) -> Tensor[(32, 1, 1, 1000), float32] {
  %0 = nn.pad(%data, 0 /* ty=int32 */, pad_width=[[0, 0], [0, 0], [0, 0], [0, 1]]) /* ty=Tensor[(32, 224, 224, 4), float32] */;
  %1 = nn.conv2d(%0, meta[relay.Constant][0] /* ty=Tensor[(7, 7, 4, 64), float32] */, strides=[2, 2], padding=[3, 3, 3, 3], channels=64, kernel_size=[7, 7], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %2 = add(%1, meta[relay.Constant][1] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %3 = nn.relu(%2) /* resnet_v1_50/conv1/Relu */ /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %4 = nn.max_pool2d(%3, pool_size=[3, 3], strides=[2, 2], padding=[0, 0, 1, 1], layout="NHWC") /* resnet_v1_50/pool1/MaxPool */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %5 = annotation.cast_hint(%4, dtype="int8") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %6 = annotation.stop_fusion(%5) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %7 = nn.conv2d(%6, meta[relay.Constant][2] /* ty=Tensor[(1, 1, 64, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %8 = add(%7, meta[relay.Constant][3] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %9 = annotation.cast_hint(%8, dtype="int8") /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %10 = annotation.cast_hint(%4, dtype="int8") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %11 = annotation.stop_fusion(%10) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %12 = nn.conv2d(%11, meta[relay.Constant][4] /* ty=Tensor[(1, 1, 64, 64), float32] */, padding=[0, 0, 0, 0], channels=64, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %13 = add(%12, meta[relay.Constant][5] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %14 = nn.relu(%13) /* resnet_v1_50/block1/unit_1/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %15 = annotation.cast_hint(%14, dtype="int8") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %16 = annotation.stop_fusion(%15) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %17 = nn.conv2d(%16, meta[relay.Constant][6] /* ty=Tensor[(3, 3, 64, 64), float32] */, padding=[1, 1, 1, 1], channels=64, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %18 = add(%17, meta[relay.Constant][7] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %19 = nn.relu(%18) /* resnet_v1_50/block1/unit_1/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %20 = annotation.cast_hint(%19, dtype="int8") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %21 = annotation.stop_fusion(%20) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %22 = nn.conv2d(%21, meta[relay.Constant][8] /* ty=Tensor[(1, 1, 64, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %23 = add(%22, meta[relay.Constant][9] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %24 = annotation.cast_hint(%23, dtype="int8") /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %25 = annotation.stop_fusion(%9) /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %26 = annotation.stop_fusion(%24) /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %27 = add(%25, %26) /* resnet_v1_50/block1/unit_1/bottleneck_v1/add */ /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %28 = nn.relu(%27) /* resnet_v1_50/block1/unit_1/bottleneck_v1/Relu */ /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %29 = annotation.cast_hint(%28, dtype="int8") /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %30 = annotation.cast_hint(%28, dtype="int8") /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %31 = annotation.stop_fusion(%30) /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %32 = nn.conv2d(%31, meta[relay.Constant][10] /* ty=Tensor[(1, 1, 256, 64), float32] */, padding=[0, 0, 0, 0], channels=64, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %33 = add(%32, meta[relay.Constant][11] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %34 = nn.relu(%33) /* resnet_v1_50/block1/unit_2/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %35 = annotation.cast_hint(%34, dtype="int8") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %36 = annotation.stop_fusion(%35) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %37 = nn.conv2d(%36, meta[relay.Constant][12] /* ty=Tensor[(3, 3, 64, 64), float32] */, padding=[1, 1, 1, 1], channels=64, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %38 = add(%37, meta[relay.Constant][13] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %39 = nn.relu(%38) /* resnet_v1_50/block1/unit_2/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %40 = annotation.cast_hint(%39, dtype="int8") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %41 = annotation.stop_fusion(%40) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %42 = nn.conv2d(%41, meta[relay.Constant][14] /* ty=Tensor[(1, 1, 64, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %43 = add(%42, meta[relay.Constant][15] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %44 = annotation.cast_hint(%43, dtype="int8") /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %45 = annotation.stop_fusion(%29) /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %46 = annotation.stop_fusion(%44) /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %47 = add(%45, %46) /* resnet_v1_50/block1/unit_2/bottleneck_v1/add */ /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %48 = nn.relu(%47) /* resnet_v1_50/block1/unit_2/bottleneck_v1/Relu */ /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %49 = nn.max_pool2d(%48, pool_size=[1, 1], strides=[2, 2], padding=[0, 0, 0, 0], layout="NHWC") /* resnet_v1_50/block1/unit_3/bottleneck_v1/shortcut/MaxPool */ /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %50 = annotation.cast_hint(%49, dtype="int8") /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %51 = annotation.cast_hint(%48, dtype="int8") /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %52 = annotation.stop_fusion(%51) /* ty=Tensor[(32, 56, 56, 256), float32] */;
  %53 = nn.conv2d(%52, meta[relay.Constant][16] /* ty=Tensor[(1, 1, 256, 64), float32] */, padding=[0, 0, 0, 0], channels=64, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %54 = add(%53, meta[relay.Constant][17] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %55 = nn.relu(%54) /* resnet_v1_50/block1/unit_3/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %56 = annotation.cast_hint(%55, dtype="int8") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %57 = annotation.stop_fusion(%56) /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %58 = nn.conv2d(%57, meta[relay.Constant][18] /* ty=Tensor[(3, 3, 64, 64), float32] */, strides=[2, 2], padding=[1, 1, 1, 1], channels=64, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 64), float32] */;
  %59 = add(%58, meta[relay.Constant][19] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 28, 28, 64), float32] */;
  %60 = nn.relu(%59) /* resnet_v1_50/block1/unit_3/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 28, 28, 64), float32] */;
  %61 = annotation.cast_hint(%60, dtype="int8") /* ty=Tensor[(32, 28, 28, 64), float32] */;
  %62 = annotation.stop_fusion(%61) /* ty=Tensor[(32, 28, 28, 64), float32] */;
  %63 = nn.conv2d(%62, meta[relay.Constant][20] /* ty=Tensor[(1, 1, 64, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %64 = add(%63, meta[relay.Constant][21] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %65 = annotation.cast_hint(%64, dtype="int8") /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %66 = annotation.stop_fusion(%50) /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %67 = annotation.stop_fusion(%65) /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %68 = add(%66, %67) /* resnet_v1_50/block1/unit_3/bottleneck_v1/add */ /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %69 = nn.relu(%68) /* resnet_v1_50/block1/unit_3/bottleneck_v1/Relu */ /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %70 = annotation.cast_hint(%69, dtype="int8") /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %71 = annotation.stop_fusion(%70) /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %72 = nn.conv2d(%71, meta[relay.Constant][22] /* ty=Tensor[(1, 1, 256, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %73 = add(%72, meta[relay.Constant][23] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %74 = annotation.cast_hint(%73, dtype="int8") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %75 = annotation.cast_hint(%69, dtype="int8") /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %76 = annotation.stop_fusion(%75) /* ty=Tensor[(32, 28, 28, 256), float32] */;
  %77 = nn.conv2d(%76, meta[relay.Constant][24] /* ty=Tensor[(1, 1, 256, 128), float32] */, padding=[0, 0, 0, 0], channels=128, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %78 = add(%77, meta[relay.Constant][25] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %79 = nn.relu(%78) /* resnet_v1_50/block2/unit_1/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %80 = annotation.cast_hint(%79, dtype="int8") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %81 = annotation.stop_fusion(%80) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %82 = nn.conv2d(%81, meta[relay.Constant][26] /* ty=Tensor[(3, 3, 128, 128), float32] */, padding=[1, 1, 1, 1], channels=128, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %83 = add(%82, meta[relay.Constant][27] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %84 = nn.relu(%83) /* resnet_v1_50/block2/unit_1/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %85 = annotation.cast_hint(%84, dtype="int8") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %86 = annotation.stop_fusion(%85) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %87 = nn.conv2d(%86, meta[relay.Constant][28] /* ty=Tensor[(1, 1, 128, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %88 = add(%87, meta[relay.Constant][29] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %89 = annotation.cast_hint(%88, dtype="int8") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %90 = annotation.stop_fusion(%74) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %91 = annotation.stop_fusion(%89) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %92 = add(%90, %91) /* resnet_v1_50/block2/unit_1/bottleneck_v1/add */ /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %93 = nn.relu(%92) /* resnet_v1_50/block2/unit_1/bottleneck_v1/Relu */ /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %94 = annotation.cast_hint(%93, dtype="int8") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %95 = annotation.cast_hint(%93, dtype="int8") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %96 = annotation.stop_fusion(%95) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %97 = nn.conv2d(%96, meta[relay.Constant][30] /* ty=Tensor[(1, 1, 512, 128), float32] */, padding=[0, 0, 0, 0], channels=128, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %98 = add(%97, meta[relay.Constant][31] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %99 = nn.relu(%98) /* resnet_v1_50/block2/unit_2/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %100 = annotation.cast_hint(%99, dtype="int8") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %101 = annotation.stop_fusion(%100) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %102 = nn.conv2d(%101, meta[relay.Constant][32] /* ty=Tensor[(3, 3, 128, 128), float32] */, padding=[1, 1, 1, 1], channels=128, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %103 = add(%102, meta[relay.Constant][33] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %104 = nn.relu(%103) /* resnet_v1_50/block2/unit_2/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %105 = annotation.cast_hint(%104, dtype="int8") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %106 = annotation.stop_fusion(%105) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %107 = nn.conv2d(%106, meta[relay.Constant][34] /* ty=Tensor[(1, 1, 128, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %108 = add(%107, meta[relay.Constant][35] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %109 = annotation.cast_hint(%108, dtype="int8") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %110 = annotation.stop_fusion(%94) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %111 = annotation.stop_fusion(%109) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %112 = add(%110, %111) /* resnet_v1_50/block2/unit_2/bottleneck_v1/add */ /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %113 = nn.relu(%112) /* resnet_v1_50/block2/unit_2/bottleneck_v1/Relu */ /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %114 = annotation.cast_hint(%113, dtype="int8") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %115 = annotation.cast_hint(%113, dtype="int8") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %116 = annotation.stop_fusion(%115) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %117 = nn.conv2d(%116, meta[relay.Constant][36] /* ty=Tensor[(1, 1, 512, 128), float32] */, padding=[0, 0, 0, 0], channels=128, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %118 = add(%117, meta[relay.Constant][37] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %119 = nn.relu(%118) /* resnet_v1_50/block2/unit_3/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %120 = annotation.cast_hint(%119, dtype="int8") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %121 = annotation.stop_fusion(%120) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %122 = nn.conv2d(%121, meta[relay.Constant][38] /* ty=Tensor[(3, 3, 128, 128), float32] */, padding=[1, 1, 1, 1], channels=128, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %123 = add(%122, meta[relay.Constant][39] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %124 = nn.relu(%123) /* resnet_v1_50/block2/unit_3/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %125 = annotation.cast_hint(%124, dtype="int8") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %126 = annotation.stop_fusion(%125) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %127 = nn.conv2d(%126, meta[relay.Constant][40] /* ty=Tensor[(1, 1, 128, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %128 = add(%127, meta[relay.Constant][41] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %129 = annotation.cast_hint(%128, dtype="int8") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %130 = annotation.stop_fusion(%114) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %131 = annotation.stop_fusion(%129) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %132 = add(%130, %131) /* resnet_v1_50/block2/unit_3/bottleneck_v1/add */ /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %133 = nn.relu(%132) /* resnet_v1_50/block2/unit_3/bottleneck_v1/Relu */ /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %134 = nn.max_pool2d(%133, pool_size=[1, 1], strides=[2, 2], padding=[0, 0, 0, 0], layout="NHWC") /* resnet_v1_50/block2/unit_4/bottleneck_v1/shortcut/MaxPool */ /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %135 = annotation.cast_hint(%134, dtype="int8") /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %136 = annotation.cast_hint(%133, dtype="int8") /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %137 = annotation.stop_fusion(%136) /* ty=Tensor[(32, 28, 28, 512), float32] */;
  %138 = nn.conv2d(%137, meta[relay.Constant][42] /* ty=Tensor[(1, 1, 512, 128), float32] */, padding=[0, 0, 0, 0], channels=128, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %139 = add(%138, meta[relay.Constant][43] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %140 = nn.relu(%139) /* resnet_v1_50/block2/unit_4/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %141 = annotation.cast_hint(%140, dtype="int8") /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %142 = annotation.stop_fusion(%141) /* ty=Tensor[(32, 28, 28, 128), float32] */;
  %143 = nn.conv2d(%142, meta[relay.Constant][44] /* ty=Tensor[(3, 3, 128, 128), float32] */, strides=[2, 2], padding=[1, 1, 1, 1], channels=128, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 128), float32] */;
  %144 = add(%143, meta[relay.Constant][45] /* ty=Tensor[(128), float32] */) /* ty=Tensor[(32, 14, 14, 128), float32] */;
  %145 = nn.relu(%144) /* resnet_v1_50/block2/unit_4/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 14, 14, 128), float32] */;
  %146 = annotation.cast_hint(%145, dtype="int8") /* ty=Tensor[(32, 14, 14, 128), float32] */;
  %147 = annotation.stop_fusion(%146) /* ty=Tensor[(32, 14, 14, 128), float32] */;
  %148 = nn.conv2d(%147, meta[relay.Constant][46] /* ty=Tensor[(1, 1, 128, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %149 = add(%148, meta[relay.Constant][47] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %150 = annotation.cast_hint(%149, dtype="int8") /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %151 = annotation.stop_fusion(%135) /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %152 = annotation.stop_fusion(%150) /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %153 = add(%151, %152) /* resnet_v1_50/block2/unit_4/bottleneck_v1/add */ /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %154 = nn.relu(%153) /* resnet_v1_50/block2/unit_4/bottleneck_v1/Relu */ /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %155 = annotation.cast_hint(%154, dtype="int8") /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %156 = annotation.stop_fusion(%155) /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %157 = nn.conv2d(%156, meta[relay.Constant][48] /* ty=Tensor[(1, 1, 512, 1024), float32] */, padding=[0, 0, 0, 0], channels=1024, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %158 = add(%157, meta[relay.Constant][49] /* ty=Tensor[(1024), float32] */) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %159 = annotation.cast_hint(%158, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %160 = annotation.cast_hint(%154, dtype="int8") /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %161 = annotation.stop_fusion(%160) /* ty=Tensor[(32, 14, 14, 512), float32] */;
  %162 = nn.conv2d(%161, meta[relay.Constant][50] /* ty=Tensor[(1, 1, 512, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %163 = add(%162, meta[relay.Constant][51] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %164 = nn.relu(%163) /* resnet_v1_50/block3/unit_1/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %165 = annotation.cast_hint(%164, dtype="int8") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %166 = annotation.stop_fusion(%165) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %167 = nn.conv2d(%166, meta[relay.Constant][52] /* ty=Tensor[(3, 3, 256, 256), float32] */, padding=[1, 1, 1, 1], channels=256, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %168 = add(%167, meta[relay.Constant][53] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %169 = nn.relu(%168) /* resnet_v1_50/block3/unit_1/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %170 = annotation.cast_hint(%169, dtype="int8") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %171 = annotation.stop_fusion(%170) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %172 = nn.conv2d(%171, meta[relay.Constant][54] /* ty=Tensor[(1, 1, 256, 1024), float32] */, padding=[0, 0, 0, 0], channels=1024, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %173 = add(%172, meta[relay.Constant][55] /* ty=Tensor[(1024), float32] */) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %174 = annotation.cast_hint(%173, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %175 = annotation.stop_fusion(%159) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %176 = annotation.stop_fusion(%174) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %177 = add(%175, %176) /* resnet_v1_50/block3/unit_1/bottleneck_v1/add */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %178 = nn.relu(%177) /* resnet_v1_50/block3/unit_1/bottleneck_v1/Relu */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %179 = annotation.cast_hint(%178, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %180 = annotation.cast_hint(%178, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %181 = annotation.stop_fusion(%180) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %182 = nn.conv2d(%181, meta[relay.Constant][56] /* ty=Tensor[(1, 1, 1024, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %183 = add(%182, meta[relay.Constant][57] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %184 = nn.relu(%183) /* resnet_v1_50/block3/unit_2/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %185 = annotation.cast_hint(%184, dtype="int8") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %186 = annotation.stop_fusion(%185) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %187 = nn.conv2d(%186, meta[relay.Constant][58] /* ty=Tensor[(3, 3, 256, 256), float32] */, padding=[1, 1, 1, 1], channels=256, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %188 = add(%187, meta[relay.Constant][59] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %189 = nn.relu(%188) /* resnet_v1_50/block3/unit_2/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %190 = annotation.cast_hint(%189, dtype="int8") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %191 = annotation.stop_fusion(%190) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %192 = nn.conv2d(%191, meta[relay.Constant][60] /* ty=Tensor[(1, 1, 256, 1024), float32] */, padding=[0, 0, 0, 0], channels=1024, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %193 = add(%192, meta[relay.Constant][61] /* ty=Tensor[(1024), float32] */) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %194 = annotation.cast_hint(%193, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %195 = annotation.stop_fusion(%179) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %196 = annotation.stop_fusion(%194) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %197 = add(%195, %196) /* resnet_v1_50/block3/unit_2/bottleneck_v1/add */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %198 = nn.relu(%197) /* resnet_v1_50/block3/unit_2/bottleneck_v1/Relu */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %199 = annotation.cast_hint(%198, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %200 = annotation.cast_hint(%198, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %201 = annotation.stop_fusion(%200) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %202 = nn.conv2d(%201, meta[relay.Constant][62] /* ty=Tensor[(1, 1, 1024, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %203 = add(%202, meta[relay.Constant][63] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %204 = nn.relu(%203) /* resnet_v1_50/block3/unit_3/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %205 = annotation.cast_hint(%204, dtype="int8") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %206 = annotation.stop_fusion(%205) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %207 = nn.conv2d(%206, meta[relay.Constant][64] /* ty=Tensor[(3, 3, 256, 256), float32] */, padding=[1, 1, 1, 1], channels=256, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %208 = add(%207, meta[relay.Constant][65] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %209 = nn.relu(%208) /* resnet_v1_50/block3/unit_3/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %210 = annotation.cast_hint(%209, dtype="int8") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %211 = annotation.stop_fusion(%210) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %212 = nn.conv2d(%211, meta[relay.Constant][66] /* ty=Tensor[(1, 1, 256, 1024), float32] */, padding=[0, 0, 0, 0], channels=1024, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %213 = add(%212, meta[relay.Constant][67] /* ty=Tensor[(1024), float32] */) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %214 = annotation.cast_hint(%213, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %215 = annotation.stop_fusion(%199) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %216 = annotation.stop_fusion(%214) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %217 = add(%215, %216) /* resnet_v1_50/block3/unit_3/bottleneck_v1/add */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %218 = nn.relu(%217) /* resnet_v1_50/block3/unit_3/bottleneck_v1/Relu */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %219 = annotation.cast_hint(%218, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %220 = annotation.cast_hint(%218, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %221 = annotation.stop_fusion(%220) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %222 = nn.conv2d(%221, meta[relay.Constant][68] /* ty=Tensor[(1, 1, 1024, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %223 = add(%222, meta[relay.Constant][69] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %224 = nn.relu(%223) /* resnet_v1_50/block3/unit_4/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %225 = annotation.cast_hint(%224, dtype="int8") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %226 = annotation.stop_fusion(%225) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %227 = nn.conv2d(%226, meta[relay.Constant][70] /* ty=Tensor[(3, 3, 256, 256), float32] */, padding=[1, 1, 1, 1], channels=256, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %228 = add(%227, meta[relay.Constant][71] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %229 = nn.relu(%228) /* resnet_v1_50/block3/unit_4/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %230 = annotation.cast_hint(%229, dtype="int8") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %231 = annotation.stop_fusion(%230) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %232 = nn.conv2d(%231, meta[relay.Constant][72] /* ty=Tensor[(1, 1, 256, 1024), float32] */, padding=[0, 0, 0, 0], channels=1024, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %233 = add(%232, meta[relay.Constant][73] /* ty=Tensor[(1024), float32] */) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %234 = annotation.cast_hint(%233, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %235 = annotation.stop_fusion(%219) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %236 = annotation.stop_fusion(%234) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %237 = add(%235, %236) /* resnet_v1_50/block3/unit_4/bottleneck_v1/add */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %238 = nn.relu(%237) /* resnet_v1_50/block3/unit_4/bottleneck_v1/Relu */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %239 = annotation.cast_hint(%238, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %240 = annotation.cast_hint(%238, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %241 = annotation.stop_fusion(%240) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %242 = nn.conv2d(%241, meta[relay.Constant][74] /* ty=Tensor[(1, 1, 1024, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %243 = add(%242, meta[relay.Constant][75] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %244 = nn.relu(%243) /* resnet_v1_50/block3/unit_5/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %245 = annotation.cast_hint(%244, dtype="int8") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %246 = annotation.stop_fusion(%245) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %247 = nn.conv2d(%246, meta[relay.Constant][76] /* ty=Tensor[(3, 3, 256, 256), float32] */, padding=[1, 1, 1, 1], channels=256, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %248 = add(%247, meta[relay.Constant][77] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %249 = nn.relu(%248) /* resnet_v1_50/block3/unit_5/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %250 = annotation.cast_hint(%249, dtype="int8") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %251 = annotation.stop_fusion(%250) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %252 = nn.conv2d(%251, meta[relay.Constant][78] /* ty=Tensor[(1, 1, 256, 1024), float32] */, padding=[0, 0, 0, 0], channels=1024, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %253 = add(%252, meta[relay.Constant][79] /* ty=Tensor[(1024), float32] */) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %254 = annotation.cast_hint(%253, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %255 = annotation.stop_fusion(%239) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %256 = annotation.stop_fusion(%254) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %257 = add(%255, %256) /* resnet_v1_50/block3/unit_5/bottleneck_v1/add */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %258 = nn.relu(%257) /* resnet_v1_50/block3/unit_5/bottleneck_v1/Relu */ /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %259 = nn.max_pool2d(%258, pool_size=[1, 1], strides=[2, 2], padding=[0, 0, 0, 0], layout="NHWC") /* resnet_v1_50/block3/unit_6/bottleneck_v1/shortcut/MaxPool */ /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %260 = annotation.cast_hint(%259, dtype="int8") /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %261 = annotation.cast_hint(%258, dtype="int8") /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %262 = annotation.stop_fusion(%261) /* ty=Tensor[(32, 14, 14, 1024), float32] */;
  %263 = nn.conv2d(%262, meta[relay.Constant][80] /* ty=Tensor[(1, 1, 1024, 256), float32] */, padding=[0, 0, 0, 0], channels=256, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %264 = add(%263, meta[relay.Constant][81] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %265 = nn.relu(%264) /* resnet_v1_50/block3/unit_6/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %266 = annotation.cast_hint(%265, dtype="int8") /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %267 = annotation.stop_fusion(%266) /* ty=Tensor[(32, 14, 14, 256), float32] */;
  %268 = nn.conv2d(%267, meta[relay.Constant][82] /* ty=Tensor[(3, 3, 256, 256), float32] */, strides=[2, 2], padding=[1, 1, 1, 1], channels=256, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 256), float32] */;
  %269 = add(%268, meta[relay.Constant][83] /* ty=Tensor[(256), float32] */) /* ty=Tensor[(32, 7, 7, 256), float32] */;
  %270 = nn.relu(%269) /* resnet_v1_50/block3/unit_6/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 7, 7, 256), float32] */;
  %271 = annotation.cast_hint(%270, dtype="int8") /* ty=Tensor[(32, 7, 7, 256), float32] */;
  %272 = annotation.stop_fusion(%271) /* ty=Tensor[(32, 7, 7, 256), float32] */;
  %273 = nn.conv2d(%272, meta[relay.Constant][84] /* ty=Tensor[(1, 1, 256, 1024), float32] */, padding=[0, 0, 0, 0], channels=1024, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %274 = add(%273, meta[relay.Constant][85] /* ty=Tensor[(1024), float32] */) /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %275 = annotation.cast_hint(%274, dtype="int8") /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %276 = annotation.stop_fusion(%260) /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %277 = annotation.stop_fusion(%275) /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %278 = add(%276, %277) /* resnet_v1_50/block3/unit_6/bottleneck_v1/add */ /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %279 = nn.relu(%278) /* resnet_v1_50/block3/unit_6/bottleneck_v1/Relu */ /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %280 = annotation.cast_hint(%279, dtype="int8") /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %281 = annotation.stop_fusion(%280) /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %282 = nn.conv2d(%281, meta[relay.Constant][86] /* ty=Tensor[(1, 1, 1024, 2048), float32] */, padding=[0, 0, 0, 0], channels=2048, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %283 = add(%282, meta[relay.Constant][87] /* ty=Tensor[(2048), float32] */) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %284 = annotation.cast_hint(%283, dtype="int8") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %285 = annotation.cast_hint(%279, dtype="int8") /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %286 = annotation.stop_fusion(%285) /* ty=Tensor[(32, 7, 7, 1024), float32] */;
  %287 = nn.conv2d(%286, meta[relay.Constant][88] /* ty=Tensor[(1, 1, 1024, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %288 = add(%287, meta[relay.Constant][89] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %289 = nn.relu(%288) /* resnet_v1_50/block4/unit_1/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %290 = annotation.cast_hint(%289, dtype="int8") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %291 = annotation.stop_fusion(%290) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %292 = nn.conv2d(%291, meta[relay.Constant][90] /* ty=Tensor[(3, 3, 512, 512), float32] */, padding=[1, 1, 1, 1], channels=512, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %293 = add(%292, meta[relay.Constant][91] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %294 = nn.relu(%293) /* resnet_v1_50/block4/unit_1/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %295 = annotation.cast_hint(%294, dtype="int8") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %296 = annotation.stop_fusion(%295) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %297 = nn.conv2d(%296, meta[relay.Constant][92] /* ty=Tensor[(1, 1, 512, 2048), float32] */, padding=[0, 0, 0, 0], channels=2048, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %298 = add(%297, meta[relay.Constant][93] /* ty=Tensor[(2048), float32] */) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %299 = annotation.cast_hint(%298, dtype="int8") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %300 = annotation.stop_fusion(%284) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %301 = annotation.stop_fusion(%299) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %302 = add(%300, %301) /* resnet_v1_50/block4/unit_1/bottleneck_v1/add */ /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %303 = nn.relu(%302) /* resnet_v1_50/block4/unit_1/bottleneck_v1/Relu */ /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %304 = annotation.cast_hint(%303, dtype="int8") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %305 = annotation.cast_hint(%303, dtype="int8") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %306 = annotation.stop_fusion(%305) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %307 = nn.conv2d(%306, meta[relay.Constant][94] /* ty=Tensor[(1, 1, 2048, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %308 = add(%307, meta[relay.Constant][95] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %309 = nn.relu(%308) /* resnet_v1_50/block4/unit_2/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %310 = annotation.cast_hint(%309, dtype="int8") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %311 = annotation.stop_fusion(%310) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %312 = nn.conv2d(%311, meta[relay.Constant][96] /* ty=Tensor[(3, 3, 512, 512), float32] */, padding=[1, 1, 1, 1], channels=512, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %313 = add(%312, meta[relay.Constant][97] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %314 = nn.relu(%313) /* resnet_v1_50/block4/unit_2/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %315 = annotation.cast_hint(%314, dtype="int8") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %316 = annotation.stop_fusion(%315) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %317 = nn.conv2d(%316, meta[relay.Constant][98] /* ty=Tensor[(1, 1, 512, 2048), float32] */, padding=[0, 0, 0, 0], channels=2048, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %318 = add(%317, meta[relay.Constant][99] /* ty=Tensor[(2048), float32] */) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %319 = annotation.cast_hint(%318, dtype="int8") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %320 = annotation.stop_fusion(%304) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %321 = annotation.stop_fusion(%319) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %322 = add(%320, %321) /* resnet_v1_50/block4/unit_2/bottleneck_v1/add */ /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %323 = nn.relu(%322) /* resnet_v1_50/block4/unit_2/bottleneck_v1/Relu */ /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %324 = annotation.cast_hint(%323, dtype="int8") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %325 = annotation.cast_hint(%323, dtype="int8") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %326 = annotation.stop_fusion(%325) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %327 = nn.conv2d(%326, meta[relay.Constant][100] /* ty=Tensor[(1, 1, 2048, 512), float32] */, padding=[0, 0, 0, 0], channels=512, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %328 = add(%327, meta[relay.Constant][101] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %329 = nn.relu(%328) /* resnet_v1_50/block4/unit_3/bottleneck_v1/conv1/Relu */ /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %330 = annotation.cast_hint(%329, dtype="int8") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %331 = annotation.stop_fusion(%330) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %332 = nn.conv2d(%331, meta[relay.Constant][102] /* ty=Tensor[(3, 3, 512, 512), float32] */, padding=[1, 1, 1, 1], channels=512, kernel_size=[3, 3], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %333 = add(%332, meta[relay.Constant][103] /* ty=Tensor[(512), float32] */) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %334 = nn.relu(%333) /* resnet_v1_50/block4/unit_3/bottleneck_v1/conv2/Relu */ /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %335 = annotation.cast_hint(%334, dtype="int8") /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %336 = annotation.stop_fusion(%335) /* ty=Tensor[(32, 7, 7, 512), float32] */;
  %337 = nn.conv2d(%336, meta[relay.Constant][104] /* ty=Tensor[(1, 1, 512, 2048), float32] */, padding=[0, 0, 0, 0], channels=2048, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %338 = add(%337, meta[relay.Constant][105] /* ty=Tensor[(2048), float32] */) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %339 = annotation.cast_hint(%338, dtype="int8") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %340 = annotation.stop_fusion(%324) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %341 = annotation.stop_fusion(%339) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %342 = add(%340, %341) /* resnet_v1_50/block4/unit_3/bottleneck_v1/add */ /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %343 = nn.relu(%342) /* resnet_v1_50/block4/unit_3/bottleneck_v1/Relu */ /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %344 = annotation.cast_hint(%343, dtype="int8") /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %345 = annotation.stop_fusion(%344) /* ty=Tensor[(32, 7, 7, 2048), float32] */;
  %346 = mean(%345, axis=[1, 2], keepdims=True) /* resnet_v1_50/pool5 */ /* ty=Tensor[(32, 1, 1, 2048), float32] */;
  %347 = nn.conv2d(%346, meta[relay.Constant][106] /* ty=Tensor[(1, 1, 2048, 1000), float32] */, padding=[0, 0, 0, 0], channels=1000, kernel_size=[1, 1], data_layout="NHWC", kernel_layout="HWIO") /* resnet_v1_50/logits/Conv2D */ /* ty=Tensor[(32, 1, 1, 1000), float32] */;
  %348 = add(%347, meta[relay.Constant][107] /* ty=Tensor[(1000), float32] */) /* resnet_v1_50/logits/BiasAdd */ /* ty=Tensor[(32, 1, 1, 1000), float32] */;
  %349 = annotation.cast_hint(%348, dtype="int8") /* ty=Tensor[(32, 1, 1, 1000), float32] */;
  annotation.stop_fusion(%349) /* ty=Tensor[(32, 1, 1, 1000), float32] */
}
```

