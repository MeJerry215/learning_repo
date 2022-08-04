# QuantizePartition

`ForwardRewriter`继承自`MixedModeMutator`。稍微回顾一下`MixedModeMutator`，通过`ExpandDataflow`将节点压栈的的方式对图进行DFS遍历，Rewrite方法，对于返回了一份对于该节点的拷贝。

这里先将一个叫做`TempRealizer`的类， 这个类的主要作用就是当访问节点的时候，如果节点是`TempExprNode`类型的，那么就调用其`Realize`函数。

常见的比如`QPartitionExprNode`，其Realize函数就是在节点后面插入`CastHint`和`StopFusion`节点。

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
        // 重新对call args调用Realize，然后设置回去，这里我没太看明白为什么要在Realize一下
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

