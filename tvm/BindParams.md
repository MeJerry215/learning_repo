# bind_params_by_name

[TOC]

作用：给指定名称的参数节点绑定上权值。

在relay中，参数通常是用`relay.var`来表示的，即输入节点，而我们的权值节点则是`relay.const`类型的。

对于任意导入的一个模型，我们怎么知道有一个输入呢，可以通过`relay.analysis.free_vars`来分析出未绑定权值的自由变量。

从任意一个前端导入模型，都会返回mod 和 params。

mod 是IRModule 是一系列的函数集合，也就是计算图，这个时候的计算图和权值是分离的，此时计算图中权值表示为`var`类型，即输入。params 是节点名和权值的字典。

在进行图优化的过程中，我们并不是很依赖权值，而是要对计算图进行变换，如果带上了权值则计算图的变化，就会相当耗费内存，如果过程中存在图的拷贝等情况。

而当build runtime的时候，这个时候就需要确定我有几个输入，哪些可以作为常量编译进模型。所以权值是必须的。

说了那么多，先看一个例子吧。

```python
x = relay.var("x")
y = relay.var("y")
z = relay.add(x, y)
f = relay.Function([x, y], z)
mod1 = tvm.IRModule.from_expr(f)
print(mod1)

'''
def @main(%x, %y) {
  add(%x, %y)
}
'''
```

图中就存在两个输入，而我们进行build的时候，如果不进行参数绑定，那么对于这个计算图的计算要的到正确的结果就需要正确的设置两个输入x和y， param即x、y。

而这个时候，我只想对x 进行加1的操作，那么这个函数只要给y绑定一个常量1即可，那么在用出来的runtime module执行的时候就只需要输入x， params即x。

```python
fbinded = relay.bind(f, {y: relay.const(1, "float32")})
mod2 = tvm.IRModule.from_expr(fbinded)
print(mod2)

'''
def @main(%x) {
  add(%x, 1f)
}
'''
```

Talk is cheap, show me the code.

```c++
relay::Function BindParamsByName(relay::Function func,
                                 const std::unordered_map<std::string, runtime::NDArray>& params) {
  std::unordered_map<std::string, relay::Var> name_dict;
  std::unordered_set<relay::Var, ObjectPtrHash, ObjectPtrEqual> repeat_var;
  /* 获取func的输入，对于重复名称的var进行标记 */
  for (auto arg : func->params) {
    const auto& name = arg->name_hint();
    if (name_dict.count(name)) {
      repeat_var.insert(name_dict[name]);
    } else {
      name_dict[name] = arg;
    }
  }

  std::unordered_map<relay::Var, Expr, ObjectPtrHash, ObjectPtrEqual> bind_dict;
  /* 从权值中查找是否存在于func的输入param, 构造Constant节点 */
  for (auto& kv : params) {
    ...
    auto arg = name_dict.at(kv.first);
    bind_dict[arg] = Constant(kv.second);
  }
  /* 绑定生成新的expr */
  Expr bound_expr = relay::Bind(func, bind_dict);
  Function ret = Downcast<Function>(bound_expr);
  return ret;
}
```

从以上代码可以知道，凡是同时出现在func的param name中以及params中key的`var`和权值则会进行绑定。

需要注意的是`bind_dict`实际上是`std::unordered_map<relay::Var, Constant>`类型的map。

```c++
Expr Bind(const Expr& expr, const tvm::Map<Var, Expr>& args_map) {
  if (const FunctionNode* func = expr.as<FunctionNode>()) {
    /* ExprBind VisitExpr的时候将在args_map 中的Var 替换为 Constant */
    Expr new_body = ExprBinder(args_map).VisitExpr(func->body);
    Array<Var> new_params;
    /* 获取所有未绑定的params */
    for (size_t i = 0; i < func->params.size(); ++i) {
      if (!args_map.count(func->params[i])) {
        new_params.push_back(func->params[i]);
      }
    }

    if (new_body.same_as(func->body) && new_params.size() == func->params.size()) {
      return expr;
    }
    /* 将新的函数body, new_params, 重新打包成Function, 这里我没太理解，为什么还要再比较一下，保险起见? */
    auto ret =
        Function(new_params, new_body, func->ret_type, func->type_params, func->attrs, func->span);
    ret->virtual_device_ = func->virtual_device();

    std::unordered_set<Var, ObjectPtrHash, ObjectPtrEqual> set;
    for (const auto& v : FreeVars(expr)) {
      set.insert(v);
    }
    for (const auto& v : FreeVars(ret)) {
      if (set.count(v) == 0) {
        new_params.push_back(v);
      }
    }

    ret =
        Function(new_params, new_body, func->ret_type, func->type_params, func->attrs, func->span);
    ret->virtual_device_ = func->virtual_device();

    return std::move(ret);
  } else {
    return ExprBinder(args_map).VisitExpr(expr);
  }
}

/*
ExprBinder重载了VisitExpr_(cont VarNode* op)
*/
Expr VisitExpr_(const VarNode* op) final {
  auto id = GetRef<Var>(op);
  auto it = args_map_.find(id);
  if (it != args_map_.end()) {
    return (*it).second; /* 替换Var为Constant */
  } else {
    return std::move(id); /* 非绑定参数原样返回 */
  }
}
```



