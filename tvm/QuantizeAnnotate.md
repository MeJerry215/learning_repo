# QuantizeAnnotate

参考API说明，主要是float32图，该pass会重写计算图图然后返回一个图来模拟当前量化产生的误差计算图。总结来说QuantizeAnnotate就是选择在合适的地方插入simulated_quantize op。

```python
def annotate():
    """Given a float32 graph, this pass will rewrite the graph and return
    a graph which simulates the error brought by the current quantization
    scheme.

    Returns
    -------
    ret: tvm.transform.Pass
        The registered pass for quantization annotation.
    """
    return _quantize.QuantizeAnnotate()
```

QuantizeAnnotate 也是使用了`ForwardRewrite`，同时提供了fmulti_ref处理函数，什么叫做multiref，就是当一个节点在DFS遍历的时候访问了两次以上就是多次引用，通常来说就是在节点产生分支之后， 在这个节点之后插入`attach_simulated_quantize`和`QAnnotateExpr`

```c++
Pass QuantizeAnnotate() {
    std::function<Expr(const Expr &)> fmulti_ref = [](const Expr &e) {
        if (e->IsInstance<TempExprNode>()) {
            const auto *n = e.as<QAnnotateExprNode>();
            ICHECK(n);
            const PackedFunc *f = runtime::Registry::Get("relay.quantize.attach_simulated_quantize");
            Expr ret = (*f)(n->expr, static_cast<int>(kQInput));
            return static_cast<Expr>(QAnnotateExpr(ret, kQInput));
        }
        return e;
    };

    runtime::TypedPackedFunc<Function(Function, IRModule, PassContext)> pass_func =
        [=](Function f, IRModule m, PassContext pc) {
        auto func = Downcast<Function>(ForwardRewrite(f, "FQAnnotateRewrite", nullptr, fmulti_ref));
        auto new_params = func->params;
        for (const auto &x : FreeVars(func)) {
            new_params.push_back(x);
        }
        return Function(new_params, func->body, func->ret_type, func->type_params, func->attrs);
    };
    return CreateFunctionPass(pass_func, 1, "QuantizeAnnotate", {});
}
```

同样的QuantizeAnnotate也是用注册的方式`register_annotate_function`

```python
'''
以下 simulated_quantize op 会以sim代替, 其括号后内的标识其是INPUT or WEIGHT or ACTIVATION or IDENTITY
conv2d

				data			weight			  data(ACTIVATION)	    weight
				 |				   |					 |			      |
			sim(INPUT)         sim(WEIGHT)           sim(INPUT)         sim(WEIGHT)
			      \               /                      \               / 
                        conv2d                                  conv2d
                          |                                       |
                QAnnotateExpr(ACTIVATION)                QAnnotateExpr(ACTIVATION)


dense
				data			weight			  data(ACTIVATION)	    weight
				 |				   |					 |			      |
			sim(INPUT)         sim(WEIGHT)           sim(INPUT)         sim(WEIGHT)
			      \               /                      \               / 
                        dense                                  dense
                          |                                       |
                QAnnotateExpr(ACTIVATION)                QAnnotateExpr(ACTIVATION)

add
				data		weight(ACTIVATION/INPUT)		data(ACTIVATION/INPUT/WEIGHT)	weight(const)
				  |             |										\						|
		       sim(INPUT)       |											\				  sim(WEIGHT)
		       		\			|												\			/
						add															add
						 |															 |
				 QAnnotateExpr(INPUT)									 QAnnotateExpr(ACTIVATION)
				 
				 
				 
				data(ACTIVATION/INPUT/WEIGHT)	weight(none const)   
						\							|
							\					sim(INPUT)
							   \				/
							   	 	  add
							   	 	   |
							   QAnnotateExpr(ACTIVATION)
							   
				data(INPUT)		weight(INPUT)			  data(ACTIVATION)		weight(ACTIVATION)
					\				/							\				/
                    	   add											add
                    		|											 |
                  QAnnotateExpr(INPUT)							QAnnotateExpr(ACTIVATION)

				data(ACTIVATION/INPUT)		weight(INPUT/ACTIVATION)
						\							/
                    	   			add	
                    			     |
                  			QAnnotateExpr(ACTIVATION)
                  
                  
identity(reshape, clip, nn.relu, strided_slice, nn.avg_pool2d, nn.batch_flatten, transpose, annotation.stop_fusion)
			data(ACTIVATION/INPUT/WEIGHT)
            			|
            		identity
            		    |
       QAnnotateExpr(ACTIVATION/INPUT/WEIGHT)
       

max_pool2d
				data(ACTIVATION)
					   |
				   sim(INPUT)
				       |
				    max_pool2d
				       |
		       QAnnotateExpr(INPUT)
		   
cast_hint
				data(ACTIVATION)
					   |
				   sim(INPUT)
				       |
				   cast_hint
				       |
		      QAnnotateExpr(INPUT)		
		      
...
				
'''
```

以一下的ir为例

```c++
  %0 = nn.pad(%data, 0 /* ty=int32 */, pad_width=[[0, 0], [0, 0], [0, 0], [0, 1]]) /* ty=Tensor[(32, 224, 224, 4), float32] */;
  %1 = nn.conv2d(%0, meta[relay.Constant][0] /* ty=Tensor[(7, 7, 4, 64), float32] */, strides=[2, 2], padding=[3, 3, 3, 3], channels=64, kernel_size=[7, 7], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %2 = add(%1, meta[relay.Constant][1] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %3 = nn.relu(%2) /* resnet_v1_50/conv1/Relu */ /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %4 = nn.max_pool2d(%3, pool_size=[3, 3], strides=[2, 2], padding=[0, 0, 1, 1], layout="NHWC") /* resnet_v1_50/pool1/MaxPool */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %5 = annotation.cast_hint(%4, dtype="int8") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %6 = annotation.stop_fusion(%5) /* ty=Tensor[(32, 56, 56, 64), float32] */;
```

两个卷积都是非QAnnotateExpr，那么就是要走sim，当遇到第一个conv的时候

```c++
  %0 = nn.pad(%data, 0 /* ty=int32 */, pad_width=[[0, 0], [0, 0], [0, 0], [0, 1]]) /* ty=Tensor[(32, 224, 224, 4), float32] */;
%1 = relay.op.annotation.simulated_quantize(%0, %dom_scale, %clip_min, %clip_max, kind=1) /* ty=Tensor[(32, 224, 224, 4), float32] */;
%2 = relay.op.annotation.simulated_quantize(meta[relay.Constant][0] /* ty=Tensor[(7, 7, 4, 64), float32] */, %dom_scale1, %clip_min1, %clip_max1, kind=2) /* ty=Tensor[(7, 7, 4, 64), float32] */;
  %3 = nn.conv2d(%1, %2 /* ty=Tensor[(7, 7, 4, 64), float32] */, strides=[2, 2], padding=[3, 3, 3, 3], channels=64, kernel_size=[7, 7], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 112, 112, 64), float32] */;
%4 = QAnnotateExpr(%3)
  %5 = add(%4, meta[relay.Constant][1] /* ty=Tensor[(64), float32] */) /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %6 = nn.relu(%5) /* resnet_v1_50/conv1/Relu */ /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %7 = nn.max_pool2d(%6, pool_size=[3, 3], strides=[2, 2], padding=[0, 0, 1, 1], layout="NHWC") /* resnet_v1_50/pool1/MaxPool */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %8 = annotation.cast_hint(%7, dtype="int8") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %9 = annotation.stop_fusion(%8) /* ty=Tensor[(32, 56, 56, 64), float32] */;
```

然后遇到add的时候

```c++
  %0 = nn.pad(%data, 0 /* ty=int32 */, pad_width=[[0, 0], [0, 0], [0, 0], [0, 1]]) /* ty=Tensor[(32, 224, 224, 4), float32] */;
%1 = relay.op.annotation.simulated_quantize(%0, %dom_scale, %clip_min, %clip_max, kind=1) /* ty=Tensor[(32, 224, 224, 4), float32] */;
%2 = relay.op.annotation.simulated_quantize(meta[relay.Constant][0] /* ty=Tensor[(7, 7, 4, 64), float32] */, %dom_scale1, %clip_min1, %clip_max1, kind=2) /* ty=Tensor[(7, 7, 4, 64), float32] */;
  %3 = nn.conv2d(%1, %2, strides=[2, 2], padding=[3, 3, 3, 3], channels=64, kernel_size=[7, 7], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 112, 112, 64), float32] */;
%4 = relay.op.annotation.simulated_quantize(meta[relay.Constant][1] /* ty=Tensor[(64), float32] */, %dom_scale2, %clip_min2, %clip_max2, kind=2) /* ty=Tensor[(64), float32] */;
  %5 = add(%3, %4) /* ty=Tensor[(32, 112, 112, 64), float32] */;
%6 = QAnnotateExpr(%5)
  %7 = nn.relu(%6) /* resnet_v1_50/conv1/Relu */ /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %8 = nn.max_pool2d(%7, pool_size=[3, 3], strides=[2, 2], padding=[0, 0, 1, 1], layout="NHWC") /* resnet_v1_50/pool1/MaxPool */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %9 = annotation.cast_hint(%8, dtype="int8") /* ty=Tensor[(32, 56, 56, 64), float32] */;
  %10 = annotation.stop_fusion(%9) /* ty=Tensor[(32, 56, 56, 64), float32] */;
```

relu是identity的op，等价于直接透传QAnnotateExpr，知道遇到最后的max_pool2d，这边参照以上的图可以得到下面的ir中关于simulated_quantize的插入方式

```c++
 %0 = nn.pad(%data, 0 /* ty=int32 */, pad_width=[[0, 0], [0, 0], [0, 0], [0, 1]]) /* ty=Tensor[(32, 224, 224, 4), float32] */;
%1 = relay.op.annotation.simulated_quantize(%0, %dom_scale, %clip_min, %clip_max, kind=1) /* ty=Tensor[(32, 224, 224, 4), float32] */;
%2 = relay.op.annotation.simulated_quantize(meta[relay.Constant][0] /* ty=Tensor[(7, 7, 4, 64), float32] */, %dom_scale1, %clip_min1, %clip_max1, kind=2) /* ty=Tensor[(7, 7, 4, 64), float32] */;
  %3 = nn.conv2d(%1, %2, strides=[2, 2], padding=[3, 3, 3, 3], channels=64, kernel_size=[7, 7], data_layout="NHWC", kernel_layout="HWIO") /* ty=Tensor[(32, 112, 112, 64), float32] */;
%4 = relay.op.annotation.simulated_quantize(meta[relay.Constant][1] /* ty=Tensor[(64), float32] */, %dom_scale2, %clip_min2, %clip_max2, kind=2) /* ty=Tensor[(64), float32] */;
  %5 = add(%3, %4) /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %6 = nn.relu(%5) /* resnet_v1_50/conv1/Relu */ /* ty=Tensor[(32, 112, 112, 64), float32] */;
%7 = relay.op.annotation.simulated_quantize(%6, %dom_scale3, %clip_min3, %clip_max3, kind=1) /* ty=Tensor[(32, 112, 112, 64), float32] */;
  %8 = nn.
      max_pool2d(%7, pool_size=[3, 3], strides=[2, 2], padding=[0, 0, 1, 1], layout="NHWC") /* resnet_v1_50/pool1/MaxPool */ /* ty=Tensor[(32, 56, 56, 64), float32] */;
%9 = relay.op.annotation.simulated_quantize(%8, %dom_scale4, %clip_min4, %clip_max4, kind=1) /* ty=Tensor[(32, 56, 56, 64), float32] */;
%10 = annotation.cast_hint(%9, dtype="int8") /* ty=Tensor[(32, 56, 56, 64), float32] */;
%11 = annotation.stop_fusion(%10) /* ty=Tensor[(32, 56, 56, 64), float32] */;
```

然后就是实体op simulated_quantize的实现，4个输入分别是dom_scale和clip_min，clip_max

```python
RELAY_REGISTER_OP("relay.op.annotation.simulated_quantize")
    .describe(R"code(simulated quantize op)code" TVM_ADD_FILELINE)
    .set_num_inputs(4)
    .add_argument("data", "Tensor", "The input data.")
    .add_argument("dom_scale", "Tensor", "The domain scale of input data. It should be a scalar")
    .add_argument("clip_min", "Tensor", "lower bound. It should be a scalar")
    .add_argument("clip_max", "Tensor", "upper bound. It should be a scalar")
    .set_attrs_type<SimulatedQuantizeAttrs>()
    .set_support_level(11)
    .add_type_rel("SimulatedQuantize", SimulatedQuantizeRel);
    
#  量化的scale为量化的阶数，divide操作相当于乘法, 经过clip min max之后，然后将数据round到整数，即输入数据就是用float表示的int8数据
@_op.register_compute("relay.op.annotation.simulated_quantize")
def simulated_quantize_compute(attrs, inputs, out_type):
    """Compiler for simulated_quantize."""
    assert len(inputs) == 4
    assert attrs.sign
    assert attrs.rounding == "round"

    data, scale, clip_min, clip_max = inputs

    if attrs.kind == QAnnotateKind.IDENTITY:
        return [topi.identity(data)]

    # simulate rounding error
    scaled_data = topi.divide(data, scale)
    clipped_data = topi.maximum(topi.minimum(scaled_data, clip_max), clip_min)
    round_data = topi.round(clipped_data)

    # recover data
    rdata = topi.multiply(round_data, scale)
    return [rdata]
```

这个op用来实现模拟量化的操作，

