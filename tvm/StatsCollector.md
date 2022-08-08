# StatsCollector

这个Pass主要是用在量化算法中对于`simulated_quantize`， 我们知道在之前的QuantizeAnnotate中插入simulated_quantize，而simulated_quantize存在4个输入节点， 对于weight不进行模拟量化，而是直接设置其属性为`kQIdentity`，这样的话其行为就是短路掉，不计算直接将输入作为输出。

我们知道weight的量化是简单的min_max的量化，而对于activation和input的量化可以使用kl_divergence和percentile。

```c++
Expr VisitExpr_(const CallNode *call) {
    Expr new_e = ExprMutator::VisitExpr_(call);
    const CallNode *new_call = new_e.as<CallNode>();
    ICHECK(new_call);
    if (new_call->op == simulated_quantize_op_) {
        auto attrs = new_call->attrs.as<SimulatedQuantizeAttrs>();
        // rewrite the annotation
        auto new_attrs = make_object<SimulatedQuantizeAttrs>();
        const Expr &quantize_input = new_call->args[0];                 // expression being quantized
        auto placeholder = MakeConstantScalar(DataType::Float(32), 0.); // unused argument
        // 
        Array<Expr> new_args{quantize_input, placeholder, placeholder, placeholder};
        new_attrs->kind = QAnnotateKind::kQIdentity;
        new_attrs->sign = attrs->sign;
        new_attrs->rounding = attrs->rounding;
        // 设置新的op simulated_quantize为identity，即输入作为输出不进行任何计算
        Expr identity_quantize = Call(new_call->op, new_args, Attrs{new_attrs}, {});
        // 非weight输出结果需要保存下来，在后续作为输出节点。
        if (attrs->kind != QAnnotateKind::kQWeight) {
            ICHECK(!quantize_input.as<ConstantNode>());
            profile_data_.push_back(identity_quantize);
        }
        return identity_quantize;
    } else {
        return new_e;
    }
}
```

总体来说这个Pass的作用就是拿到`simulated_quantize`的输出结果，或者说是dump网络的中间数据。
