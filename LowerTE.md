# LowerTE

![image-20220808184616435](D:\Repo\learning_repo\LowerTE.assets\image-20220808184616435.png)

relay ir 经过这个pass之后最主要的变化是从IR转换为TIR。

```c++
IRModule LowerTE(const IRModule &module, const String &module_name, ProcessFn process_fn,
                 SEScope host_se_scope) {
    TECompiler compiler(module);
	// lower te to tir
    IRModule updated_module = LowerTensorExpr(module_name, compiler, std::move(process_fn),
                                              std::move(host_se_scope))(module);

    compiler->AddExterns(updated_module);

    IRModule lowered_module = compiler->GetLoweredFunctions();
    for (const auto &kv : lowered_module->functions) {
        updated_module->Add(kv.first, kv.second);
    }

    Array<runtime::Module> external_mods =
        module->GetAttr<Array<runtime::Module>>("external_mods", Array<runtime::Module>()).value();
    Array<runtime::Module> new_external_mods = compiler->LowerExternalFunctions();
    for (const auto &mod : new_external_mods) {
        external_mods.push_back(mod);
    }

    Map<GlobalVar, String> device_contexts =
        module->GetAttr<Map<GlobalVar, String>>("device_contexts", Map<GlobalVar, String>()).value();
    Map<GlobalVar, String> new_device_contexts = compiler->GetDeviceContexts();
    for (const auto &kv : new_device_contexts) {
        ICHECK_EQ(device_contexts.count(kv.first), 0);
        device_contexts.Set(kv.first, kv.second);
    }

    updated_module = WithAttrs(updated_module, {{"external_mods", std::move(external_mods)},
                                                {"device_contexts", std::move(device_contexts)}});

    if (backend::IsAutoSchedulerEnabled()) {
        Map<String, Integer> op_weights =
            module->GetAttr<Map<String, Integer>>("op_weights", Map<String, Integer>()).value();
        Map<String, Integer> new_op_weights = compiler->GetOpWeights();
        for (const auto &kv : new_op_weights) {
            ICHECK_EQ(op_weights.count(kv.first), 0);
            op_weights.Set(kv.first, kv.second);
        }
        updated_module = WithAttr(updated_module, "op_weights", std::move(op_weights));
    }
    return updated_module;
}
```

LowerTensorExpr是一个Pass，负责将Relay转化为TIR，其具体实现为`LowerTensorExprMutator`， 其父类`DeviceAwareExprMutator`继承了`ExprMutator`和`LexicalOnDeviceMixin`，在之前我们知道`ExprMutator`是dfs遍历节点的修改器，而`LexicalOnDeviceMixin`则是function关联到设备上的遍历器。

在`DeviceAwareExprMutator`中其主要对于三种节点使用`VisitExpr_`进行了访问`FunctionNode`、`CallNode`、`LetNode`。而在`FunctionNode`、`CallNode`、本质上调用了`DeviceAwareVisitExpr_`的实现，而对于`LetNode`的访问则是调用的函数就相对来说较多有4个。主要可以参考下面的

```c++
class DeviceAwareExprMutator : public ExprMutator, public LexicalOnDeviceMixin
{
public:
    explicit DeviceAwareExprMutator(const Optional<IRModule> &maybe_mod)
        : LexicalOnDeviceMixin(maybe_mod) {}

    Expr VisitExpr_(const FunctionNode *function_node) final;
    virtual Expr DeviceAwareVisitExpr_(const FunctionNode *function_node);

    Expr VisitExpr_(const CallNode *call_node) final;
    virtual Expr DeviceAwareVisitExpr_(const CallNode *call_node);

    Expr VisitExpr_(const LetNode *let_node) final;
    virtual void PreVisitLetBlock_(const LetNode *let_node);
    virtual std::pair<Var, Expr> PreVisitLetBinding_(const Var &var, const Expr &value);
    virtual Expr PostVisitLet_(const LetNode *pre_let_node, const LetNode *post_let_node);
    virtual Expr PostVisitLetBlock_(const LetNode *pre_let_node, const LetNode *post_let_node);
};
```

所以现在我们需要知道什么是Let、Function、和Call。

以下面的mod举例, 这是low前的relay优化完的mod

```c++
def @main /* id=139556496 */(%data: Tensor[(10), float32], param_se_scopes=[SEScope(device_type=20, virtual_device_id=0, target=Target(kind='T4', keys={'T4', 'gpu'}, attrs={'max_num_threads': 2048, 'thread_warp_size': 64, 'mtriple': "T4", 'shared_memory_per_block': 131072, 'max_threads_per_block': 2048, 'mcpu': "ivcore11", 'registers_per_block': 262144}, host=Target(kind='llvm', keys={'cpu'}, attrs={'link-params': (bool)0}, id=139398992), id=139401952), id=139739560)], result_se_scope=SEScope(device_type=20, virtual_device_id=0, target=Target(kind='T4', keys={'T4', 'gpu'}, attrs={'max_num_threads': 2048, 'thread_warp_size': 64, 'model': "BI", 'mtriple': "T4", 'shared_memory_per_block': 131072, 'max_threads_per_block': 2048, 'mcpu': "ivcore11", 'registers_per_block': 262144}, host=Target(kind='llvm', keys={'cpu'}, attrs={'link-params': (bool)0}, id=139398992), id=139401952), id=139739560), hash="090f2034c3f28d6e") -> Tensor[(10), float32] {
  %2 = fn (%p0: Tensor[(10), float32], %p1: Tensor[(10), float32], Primitive=1, hash="300cd5f77d071fde") -> Tensor[(10), float32] {
    %0 = divide(%p0, %p1) /* ty=Tensor[(10), float32] */;
    %1 = multiply(%0, 2f /* ty=float32 */) /* ty=Tensor[(10), float32] */;
    nn.relu(%1) /* ty=Tensor[(10), float32] */
  };
  %2(%data, meta[relay.Constant][0] /* ty=Tensor[(10), float32] */) /* ty=Tensor[(10), float32] */
}
```

Call顾名思义就是一次函数调用， 就是`divide(%p0, %p1)`和`multilpy(%0, 2f)`，以及`nn.relu(%1)`。

然后是Let，Let通常体现为赋值操作，比如`%0 = divide(%p0, %p1)`， 对于Let只有var，value，body三个属性，实际在使用过程中是将value绑定到var，或者是将body绑定的到var。如果将value绑定到var，则value通常来说是个常量，而body绑定到var，则body通常是一个计算表达式，可以是function

```c++
class LetNode : public ExprNode {
 public:
  /*! \brief The variable we bind to */
  Var var;
  /*! \brief The value we bind var to */
  Expr value;
  /*! \brief The body of the let binding */
  Expr body;
}
```

最后到了我们的Function，Function就如同我们的正常c++代码中的函数定义，包含了入参params，函数体body，返回值类型ret_type，入参类型type_params。在relay图中，看到如`%2 = fn(params...type_params...) -> ret_type { body }`形式的就是一个Function，这里的Function不包含`%2 =`的赋值操作。

```c++
class FunctionNode : public BaseFuncNode {
 public:
  tvm::Array<Var> params;
  Expr body;
  Type ret_type;
  tvm::Array<TypeVar> type_params;
}
```

OK， 在讲解了基础概念之后，我们就可以深入一点看看这个Pass做了什么，首先是对于CallNode，这是最基础的遍历。

```c++
Expr DeviceAwareExprMutator::VisitExpr_(const CallNode* call_node) {
  // 获取当前节点的OnDevice属性，简单的说，就是如果当前节点已经确定是在是某个device上的则使用对应的device属性，否则使用没有属性
  OnDeviceProps props = GetOnDeviceProps(call_node);
  if (props.body.defined() && props.is_fixed) {
    // 进入device作用域
    PushSEScope(props.se_scope);
    Expr expr = VisitExpr(props.body);
    // 离开device作用于
    PopSEScope();
    return MaybeOnDevice(expr, props.se_scope, props.is_fixed);
  } else {
    // 没有属性直接遍历
    return DeviceAwareVisitExpr_(call_node);
  }
}
```

`FunctionNode`的访问也是相似，不过其存在函数调用深度的计数。

现在咱们看LowerTensorExprMutator对于`CallNode`是如何访问的

```c++
Expr DeviceAwareVisitExpr_(const CallNode *call_node) override
{
    // We can see five forms of calls:
    //  1. A 'normal' Relay call to a Function with the "primitive" attribute. We will need
    //     to lower that to a global PrimFunc and rewrite the call to:
    //       call_lowered(@new_global, (arg1, ..., argn), <attributes>)
    //     However there are a few special forms which are excluded from this treatment, see
    //     below.
    //  2. A 'normal' Relay call to a Function with the "compiler" attribute. We will need
    //     to invoke the appropriate BYOC toolchain function to yield a runtime module and
    //     rewrite the call to the same form as above.
    //  3. A 'normal' Relay call to a PrimFunc which has already been supplied via a global
    //     definition. We rewrite to use the call_lowered form, but otherwise nothing else
    //     needs to be done.
    //  4. A 'normal' Relay call to a Relay Function without any special attribute. These
    //     calls are not changed.
    //  5. A call_lowered call from an earlier invocation of this pass.
    // Note that ResolveToPrimitive will yield non-null only for cases 1-3.

    // Look for (possibly indirect) calls to primitives.
    BaseFunc primitive_func = ResolveToPrimitive(call_node->op);
    if (!primitive_func.defined()) {
        // Not a call to a primitive function we need to rewrite.
        if (const auto *function_node = call_node->op.as<FunctionNode>()) {
            process_fn_(GetRef<Function>(function_node));
        }
        return DeviceAwareExprMutator::DeviceAwareVisitExpr_(call_node);
    }

    // Prepare the arguments.
    Array<Expr> new_args;
    for (const auto &arg : call_node->args) {
        new_args.push_back(VisitExpr(arg));
    }

    // Special case: device_copies are left as calls to primitive operators
    // (thus undoing FuseOps) so that each backend can handle them directly.
    // TODO(mbs): device_copy cleanup. Would be better for FuseOps to just leave device_copy alone.
    if (const auto *function_node = primitive_func.as<FunctionNode>()) {
        DeviceCopyProps device_copy_props = GetDeviceCopyProps(function_node->body);
        if (device_copy_props.body.defined()) {
            ICHECK_EQ(new_args.size(), 1);
            return DeviceCopy(new_args[0], device_copy_props.src_se_scope,
                              device_copy_props.dst_se_scope);
        }
    }

    // Special case: If already lowered by other means then so we don't need to mutate
    // the call but we do need to mutate the arguments
    if (const auto *prim_func_node = primitive_func.as<tir::PrimFuncNode>()) {
        // Function should already be Target annotated by this point
        // but the TE Compiler metadata is still needed for the callback
        // TODO(Mousius) - Robustify this to not assume we're in the GlobalVar for Target Hooks
        GlobalVar prim_func_var = Downcast<GlobalVar>(call_node->op);
        tir::PrimFunc prim_func = GetRef<tir::PrimFunc>(prim_func_node);

        Map<GlobalVar, tir::PrimFunc> prim_fns = {{prim_func_var, prim_func}};
        tir::PrimFunc func_with_metadata = WithAttrs(prim_func, {
                                                                    {"prim_fn_var", prim_func_var},
                                                                    {"prim_funcs", prim_fns},
                                                                });

        ICHECK(!IsDynamic(call_node->checked_type()));
        auto call_lowered_attrs = make_object<CallLoweredAttrs>();
        call_lowered_attrs->metadata.Set("relay_attrs", primitive_func->attrs);

        process_fn_(func_with_metadata);
        return CallLowered(call_node->op, std::move(new_args), Attrs(std::move(call_lowered_attrs)),
                           call_node->type_args, call_node->span);
    }

    // Typical case: call to fused primitive Relay Function.
    // Find the desired target device.
    Target target;
    if (primitive_func->GetAttr<String>(attr::kCompiler).defined()) {
        // The generic 'external device' target.
        // TODO(mbs): Retire once replaced unified BYOC compiler and target machinery
        target = Target("ext_dev");
    } else {
        // The target corresponding to the call_node expression's annotation.
        SEScope se_scope = GetSEScope(GetRef<Call>(call_node));
        ICHECK(!se_scope->IsFullyUnconstrained());
        target = se_scope->target;
        ICHECK(target.defined());
    }

    // Lower the primitive function for that target.
    Function function = Downcast<Function>(primitive_func);
    return MakeLoweredCall(function, std::move(new_args), call_node->type_args, call_node->span,
                           target);
}
```

