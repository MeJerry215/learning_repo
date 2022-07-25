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

`ExprFunctor`内部维护了虚函数表，每个Node都存在type_index，根据node注册的type_index选择合适的的VisitExpr函数执行。在这个类中并没有进行遍历的逻辑，而是定义方法行为。这里看另外一个实现
