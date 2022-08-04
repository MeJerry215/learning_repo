# QuantizeAnnotate

参考API说明，主要是float32图，该pass会重写计算图图然后返回一个图来模拟当前量化产生的误差计算图。

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
			
'''
```

