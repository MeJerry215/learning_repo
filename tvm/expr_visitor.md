# Expr Visitor

[TOC]

本篇主要讲解TVM中如何进行图的遍历访问。

参考[TVM之设计模式解读（一）--visitor模式](https://zhuanlan.zhihu.com/p/341334406)



对于表达式的访问类的基类都是`ExprFunctor`

```c++
template <typename R, typename... Args>
class NodeFunctor<R(const ObjectRef& n, Args...)> {
 private:
  typedef R (*FPointer)(const ObjectRef& n, Args...);
  using TSelf = NodeFunctor<R(const ObjectRef& n, Args...)>;
  std::vector<FPointer> func_;
 public:
  R operator()(const ObjectRef& n, Args... args) const {
    return (*func_[n->type_index()])(n, std::forward<Args>(args)...);
  }
}

template <typename R, typename... Args>
class ExprFunctor<R(const Expr& n, Args...)> {
public:
  using FType = tvm::NodeFunctor<R(const ObjectRef& n, TSelf* self, Args...)>;
  virtual R VisitExpr(const Expr& n, Args... args) {
    static FType vtable = InitVTable();
    return vtable(n, this, std::forward<Args>(args)...);
  }
    
  virtual R VisitExpr_(const ConstantNode* op, Args... args) EXPR_FUNCTOR_DEFAULT;
  virtual R VisitExpr_(const TupleNode* op, Args... args) EXPR_FUNCTOR_DEFAULT;
  virtual R VisitExpr_(const VarNode* op, Args... args) EXPR_FUNCTOR_DEFAULT;
...

private:
  // initialize the vtable.
  static FType InitVTable() {
    FType vtable;
    RELAY_EXPR_FUNCTOR_DISPATCH(ConstantNode);
    RELAY_EXPR_FUNCTOR_DISPATCH(TupleNode);
    RELAY_EXPR_FUNCTOR_DISPATCH(VarNode);
	...
    return vtable;
  }
}
```

`ExprFunctor`内部维护了虚函数表，每个Node都存在type_index，根据node注册的type_index选择合适的的VisitExpr函数执行。在这个类中并没有进行遍历的逻辑，而是定义方法行为。这里看另外一个实现`ExprVisitor`，如果要对特定属性进行访问可以继承`ExprVisitor`，默认实现`ExprVisitor`访问了所有结点的所有属性。

```c++
class ExprVisitor : public ::tvm::relay::ExprFunctor<void(const Expr& n)> {
 public:
  void VisitExpr(const Expr& expr) override;
  void VisitExpr_(const VarNode* op) override;
  void VisitExpr_(const GlobalVarNode* op) override;
  void VisitExpr_(const ConstantNode* op) override;
  void VisitExpr_(const TupleNode* op) override;
  void VisitExpr_(const FunctionNode* op) override;
  void VisitExpr_(const CallNode* op) override;
  void VisitExpr_(const LetNode* op) override;
  void VisitExpr_(const IfNode* op) override;
  void VisitExpr_(const OpNode* op) override;
  void VisitExpr_(const TupleGetItemNode* op) override;
  void VisitExpr_(const RefCreateNode* op) override;
  void VisitExpr_(const RefReadNode* op) override;
  void VisitExpr_(const RefWriteNode* op) override;
  void VisitExpr_(const ConstructorNode* op) override;
  void VisitExpr_(const MatchNode* op) override;

 protected:
  // Internal visiting counter
  std::unordered_map<const Object*, size_t> visit_counter_;
};
```

首先看`VisitExpr`方法，VisitExpr方法对于访问进行了计数，如果op访问过则计数加一，没有访问过则走父类的虚函数表执行对应的VisitExpr_函数。

```c++
void ExprVisitor::VisitExpr(const Expr& expr) {
  auto it = visit_counter_.find(expr.get());
  if (it != visit_counter_.end()) {
    ++it->second;
  } else {
    using TParent = ExprFunctor<void(const Expr&)>;
    TParent::VisitExpr(expr);
    visit_counter_.insert({expr.get(), 1});
  }
}
```

这里简单看一下某几种op的实现

```c++
void ExprVisitor::VisitExpr_(const CallNode* op) {
  this->VisitSpan(op->span);
  this->VisitExpr(op->op);

  for (auto ty_arg : op->type_args) {
    this->VisitType(ty_arg);
  }

  for (auto arg : op->args) {
    this->VisitExpr(arg);
  }
}

void ExprVisitor::VisitExpr_(const VarNode* op) {
  this->VisitSpan(op->span);
  if (op->type_annotation.defined()) {
    this->VisitType(op->type_annotation);
  }
}
```

对特定的节点访问其内部所有节点。所以默认的ExprVisitor的功能，除了op计数没有其他功能了。

当要进行扩展的时候，继承ExprVisitor，然后重写你感兴趣的节点的`VisitExpr_`函数即可。

```c++
/*
UseVar 只关注Var是否使用, 如果访问到VarNode use_var就设置为true.
遍历结束之后没有访问到var, var就为false.
*/
class UseVarVisitor : public ExprVisitor {
 public:
  explicit UseVarVisitor(const Var& v) : v(v) {}

  static bool UseVar(const Var& v, const Expr& e) {
    UseVarVisitor uv(v);
    uv(e);
    return uv.use_var;
  }

 private:
  bool use_var = false;
  Var v;

  void VisitExpr_(const VarNode* vn) override { use_var = use_var || (v == GetRef<Var>(vn)); }
};
```



