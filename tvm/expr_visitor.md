# Expr Visitor

[TOC]

本篇主要讲解TVM中如何进行图的遍历访问。

参考[TVM之设计模式解读（一）--visitor模式](https://zhuanlan.zhihu.com/p/341334406)

## ExprFunctor

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

`ExprFunctor`内部维护了虚函数表，每个Node都存在type_index，根据node注册的type_index选择合适的的VisitExpr函数执行。在这个类中并没有进行遍历的逻辑，而是定义方法行为。

## ExprVisitor

这里看另外一个实现`ExprVisitor`，如果要对特定属性进行访问可以继承`ExprVisitor`，默认实现`ExprVisitor`访问了所有结点的所有属性。

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

首先看`VisitExpr`方法，VisitExpr方法对于访问进行了计数，如果op访问过则计数加一，**没有访问过则走父类的虚函数表执行对应的VisitExpr_函数**。

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
class CallNode : public ExprNode {
 public:
  Expr op;
  tvm::Array<relay::Expr> args;
  Attrs attrs;
  tvm::Array<Type> type_args;
}

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

这里有个注意点就是当执行具体VisitExpr_的时候，还原成对应Node实体的节点指针。

```c++
#define RELAY_EXPR_FUNCTOR_DISPATCH(OP)                                                    \
  vtable.template set_dispatch<OP>([](const ObjectRef& n, TSelf* self, Args... args) {     \
    return self->VisitExpr_(static_cast<const OP*>(n.get()), std::forward<Args>(args)...); \
  });
```

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

## ExprMutator

既然存在访问，那么就存在对Expr的改写，`ExprMutator`就是处理这种事情。`ExprMutator`其主要实现在VisitExpr

```c++
Expr ExprMutator::VisitExpr(const Expr& expr) {
  auto it = this->memo_.find(expr);
  if (it != this->memo_.end()) {
    return it->second;
  } else {
    Expr new_expr = ExprFunctor::VisitExpr(expr);
    memo_[expr] = new_expr;
    return new_expr;
  }
}
```

VisitExpr的memo 以及返回的都是new_expr，那么什么是new_expr。new_expr是你对expr本身做的一些修改，然后将新的节点返回回去。

```c++
Expr ExprMutator::VisitExpr_(const ConstantNode* op) { return GetRef<Expr>(op); }
```

因为Expr进来的时候进行了一次脱壳处理，即从Expr 转换为ExprNode，现在就是将ExprNode再次转换为Expr回去。**GetRef总是指向同一元素，返回的Expr实际指向的数据指针仍然是op**。

同样的是看一个简单的派生类`CastCanonicalizer`的实现，其只扩展了对于`CallNode`的实现，首先为什么会存在这样的实现，我们希望网络中总是传输较少的数据，这样可以节省带宽，如果正常场景下的融合，如下图左半部分，则conv+cast 融合，网络输出的是int32的数据。而转换为右图之后，conv的输出int8的结果，而融合节点下移变为cast + log、cast + relu的方式。这个Pass的核心思想是减少数据传输。如果以这个角度看

```c++
/*
                conv                  conv
                  | int8              /   \
                cast        --->   cast   cast
                /   \                |     |
              log   relu            log    relu
                \   /                 \   /
                 add                   add
*/

Expr VisitExpr_(const CallNode *call) {
    static auto fpattern = Op::GetAttrMap<TOpPattern>("TOpPattern");

    if (const OpNode *opnode = call->op.as<OpNode>()) {
        auto pattern = fpattern[GetRef<Op>(opnode)];
        // 当前Op和之前的Op能够融合，主要考虑的是和前面一层的cast融合，所以有这个判断。
        if (pattern <= kBroadcast) {
            Array<Expr> call_args = call->args;
            bool unchanged = true;
            // 先遍历本Op的输入Op
            for (size_t i = 0; i < call_args.size(); ++i) {
                Expr arg = call_args[i];
                Expr new_arg = GetNewCallArg(arg);
                // 如果输入参数和产生的新参数不同则设置为新的参数
                if (!arg.same_as(new_arg)) {
                    call_args.Set(i, new_arg);
                    unchanged = false;
                }
            }
            // 如果没有改变则直接返回当前的封装，对图结构没有影响
            if (unchanged) {
                return GetRef<Expr>(call);
            }
            // 图发生了改变，重新构造一个新的Call, 主要是修改当前的输入，导致的入参改变
            return Call(call->op, call_args, call->attrs, call->type_args);
        }
    }

    Expr new_expr = ExprMutator::VisitExpr_(call);
    return new_expr;
}


Expr GetNewCallArg(const Expr &e) {
    // 先访问当前节点之前的节点DFS, 如果之前的节点发生改变那么就是产生了new expr, 没有就是没有产生新的节点等同于e
    Expr new_expr = this->VisitExpr(e);

    if (const CallNode *call = e.as<CallNode>()) {
        // 只处理cast节点
        if (call->op == cast_op_) {
            auto attrs = call->attrs.as<CastAttrs>();
            const auto *from_type = call->args[0]->type_as<TensorTypeNode>();
            ICHECK(from_type);
			// 只有低bit转换到高bit才有需要做转换
            if (from_type->dtype.bits() < attrs->dtype.bits()) {
                // 如果当前访问超过1次对于当前节点，则需要创建一个对于当前节点的拷贝，则产生一个分支。
                if (++ref_counter_[call] > 1) {
                    const CallNode *new_call = new_expr.as<CallNode>();
                    ICHECK(new_call);
                    ICHECK(new_call->op == cast_op_);
                    return Call(new_call->op, new_call->args, new_call->attrs, new_call->type_args);
                }
            }
        }
    }
    return new_expr;
}
```



## MixedModeVisitor

在以上的`ExprVisitor`和`ExprFunctor`中，由于是递归调用的，当遇到大的模型时候，递归带来一个问题就是会爆栈如果深度太深。所以这就引入了后面的两个类，在遍历的时候扩展节点，减少递归深度。对于节点访问存在visit_limit的限制，每个节点最多访问visit_limit次数。

```c++
class MixedModeVisitor : public ::tvm::relay::ExprVisitor
{
public:
    explicit MixedModeVisitor(int visit_limit = 1);

    void VisitExpr(const Expr &expr) final;
    void VisitExpr_(const CallNode *op) override;
    void VisitExpr_(const TupleNode *op) override;
    void VisitExpr_(const TupleGetItemNode *op) override;

protected:

    virtual void VisitLeaf(const Expr &expr);

    virtual bool CheckVisited(const Expr &expr);

    size_t visit_limit_;
};
```

其中重写的三个VistExpr_函数对于CallNode、TupleNode、TupleGetItemNode没有实现，其他没有重写的函数都是集成自ExprVisitor将个个Node的元素访问一遍。重点需要关注的三个函数`VisitExpr`、`VisitLeaf`、`CheckVisited`。

`MixedModeVisitor`自带备忘录memo，来统计节点访问的次数。当访问节点时，首先需要判断是否超过visit_limit，没超过则调用ExprFunctor中的虚函数表进行访问。

```c++
bool MixedModeVisitor::CheckVisited(const Expr& expr) {
  if (visit_counter_[expr.get()] < visit_limit_) {
    return false;
  } else {
    visit_counter_[expr.get()]++;
    return true;
  }
}

void MixedModeVisitor::VisitLeaf(const Expr& expr) {
  if (visit_counter_[expr.get()] < visit_limit_) {
    ExprFunctor::VisitExpr(expr);
  }
  visit_counter_[expr.get()]++;
}
```

其最为重要的一个方法为`Visit_Expr`，将checkVisited函数以及VisitLeaf传给ExpandDataflow，然后访问节点。

```c++
template <typename FCheckVisited, typename FVisitLeaf>
void ExpandDataflow(Expr expr, FCheckVisited fcheck_visited, FVisitLeaf fvisit_leaf) {
  auto fexpand_expr = [](const Expr& expr) {
    std::vector<Expr> result;
    if (const CallNode* op = expr.as<CallNode>()) {
      for (auto it = op->args.rbegin(); it != op->args.rend(); ++it) {
        result.push_back(*it);
      }
      result.push_back(op->op);
    } else if (const TupleNode* op = expr.as<TupleNode>()) {
      for (auto it = op->fields.rbegin(); it != op->fields.rend(); ++it) {
        result.push_back(*it);
      }
    } else if (const TupleGetItemNode* op = expr.as<TupleGetItemNode>()) {
      result.push_back(op->tuple);
    }
    return result;
  };
  ExpandDataflow(expr, fcheck_visited, fvisit_leaf, fexpand_expr);
}

void MixedModeVisitor::VisitExpr(const Expr& expr) {
  auto fcheck_visited = [this](const Expr& expr) { return this->CheckVisited(expr); };
  auto fvisit_leaf = [this](const Expr& expr) { return this->VisitLeaf(expr); };
  if (visit_counter_[expr.get()] < visit_limit_) {
    ExpandDataflow(expr, fcheck_visited, fvisit_leaf);
  }
}
```

在`ExpandDataflow`中定义了如何扩展节点的lamda函数`fexpand_expr`来扩展三种类型的节点：`CallNode`、`TupleNode`、`TupleGetItemNode`。

在主流程`ExpandDataflow`中

```c++
template <typename FCheckVisited, typename FVisitLeaf, typename FExpandExpr>
void ExpandDataflow(Expr expr, FCheckVisited fcheck_visited, FVisitLeaf fvisit_leaf,
                    FExpandExpr fexpand_expr) {
  std::deque<v_info> stack;
  // lamda函数定义了, 如果节点没有访问，压入栈中，栈先进后出，expr节点会最后访问到，而其前置节点都会在这之前访问
  auto fpush_to_stack = [&fcheck_visited, &stack](const Expr& expr) {
    if (!fcheck_visited(expr)) {
      stack.emplace_front(v_info(expr));
    }
  };

  fpush_to_stack(expr);
  // 如果栈中还存在元素没有遍历
  while (stack.size() > 0) {
    v_info* front = &stack.front();
    if (fcheck_visited(front->node)) { // 如果该元素已经访问, 则直接出栈
      stack.pop_front();
    } else if (front->children_expanded) { // 如果当前节点的子节点都已经展开，即已经遍历过，可以遍历本节点
      fvisit_leaf(front->node);
      stack.pop_front();
    } else {
      // 设置当前子节点展开标志，并将所有的子节点压栈，后进先出，当子节点全部遍历过后，则当前节点可以出栈。
      front->children_expanded = true;
      for (auto e : fexpand_expr(front->node)) {
        fpush_to_stack(e);
      }
    }
  }
}
```

`MixedModeVistor`主要是将dfs的访问模式改变使用栈循环的访问模式，避免了调用栈过深导致的爆栈的可能。

## MixedModeMutator

`MixedModeMutator`支持修改节点的遍历器。本质上还是`MixedModeMutator`，只是在原来的`VisitExpr_`里面增加了`Rewrite`的逻辑。

```c++
Expr VisitExpr_(const CallNode *call_node) final { return Rewrite(call_node); };

virtual Expr Rewrite_(const CallNode *pre, const Expr &post) { return post; }

template <typename T>
Expr Rewrite(const T *op) {
    Expr post = ExprMutator::VisitExpr_(op);
    return Rewrite_(op, post);
}
```

当前的实现中，就是使用`ExprMutator::VisitExpr_`生成的新的Expr去重写。所以直接使用这个`MixedModeMutator`得到的结果就是原图节点的一份拷贝。

在使用以上的Vistor和Mutator更多的是一种约定，就是约定不修改节点而只是遍历，或者是修改节点。

这里的举例`InferenceSimplifier`只重载了`TupleGetItemNode`和`CallNode`的`Rewrite_`函数。这个Pass主要的功能是，简化norm类的op，同时去除掉dropout。。。为什么他要多加进去一个干掉dropout

```c++
Expr Rewrite_(const TupleGetItemNode *n, const Expr &new_e) final
{
    const auto *new_n = new_e.as<TupleGetItemNode>();
    if (new_n->index != 0) {
        return new_e;
    }
    if (const auto *call = new_n->tuple.as<CallNode>()) {
        if (call->op == batch_norm_op_) {
            return BatchNormToInferUnpack(call->attrs, call->args[0], call->args[1], call->args[2],
                                          call->args[3], call->args[4], ty_map_.at(call->args[0]));
        }
        else if (call->op == dropout_op_) {
            return call->args[0];
        }
    }
    return new_e;
}

Expr Rewrite_(const CallNode *n, const Expr &new_n) {
    if (n->op == batch_norm_op_) {
        ty_map_[new_n.as<CallNode>()->args[0]] = n->args[0]->checked_type();
    }
    else if (n->op == layer_norm_op_) {
        const auto *call = new_n.as<CallNode>();
        return LayerNormToInferUnpack(call->attrs, call->args[0], call->args[1], call->args[2],
                                      n->args[0]->checked_type());
    } else if (n->op == group_norm_op_) {
        const auto *call = new_n.as<CallNode>();
        return GroupNormToInferUnpack(call->attrs, call->args[0], call->args[1], call->args[2],
                                      n->args[0]->checked_type());
    }
    else if (n->op == instance_norm_op_) {
        const auto *call = new_n.as<CallNode>();
        return InstanceNormToInferUnpack(call->attrs, call->args[0], call->args[1], call->args[2],
                                         n->args[0]->checked_type());
    }
    else if (n->op == l2_norm_op_) {
        const auto *call = new_n.as<CallNode>();
        return L2NormToInferUnpack(call->attrs, call->args[0]);
    }
    return new_n;
}
```

这里的`Rewrite_`主要就是将原有的计算逻辑给替换成其他op的组和计算，主要是为了后续的表达式消除以及一些其他的pass优化。

## ExprRewriter

 表达式重写，更加类似于ExprFunctor，只不过其将所有的逻辑替换为`Rewrite_`，内部也有一个`Rewrite_`的虚函数表，对于某类关注的Op重写。



以上就是在tvm expr_functor中存在的图访问的，主要模式。
