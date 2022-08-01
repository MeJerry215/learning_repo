# Combine Parallel Dense

首先看一个图

```
before combine paralle dense def @main /* id=94694752 */(%data: Tensor[(1, 16), float32]) -> Tensor[(1, 32), int32] {
  %0 = nn.dense(%data, meta[relay.Constant][0] /* ty=Tensor[(32, 16), float32] */, units=None) /* ty=Tensor[(1, 32), float32] */;
  %1 = nn.relu(%0) /* ty=Tensor[(1, 32), float32] */;
  %2 = nn.dense(%data, meta[relay.Constant][1] /* ty=Tensor[(32, 16), float32] */, units=None) /* ty=Tensor[(1, 32), float32] */;
  %3 = nn.relu(%2) /* ty=Tensor[(1, 32), float32] */;
  %4 = abs(%1) /* ty=Tensor[(1, 32), float32] */;
  %5 = abs(%3) /* ty=Tensor[(1, 32), float32] */;
  %6 = add(%4, %5) /* ty=Tensor[(1, 32), float32] */;
  cast(%6, dtype="int32") /* ty=Tensor[(1, 32), int32] */
}


after combine paralle dense def @main /* id=94694752 */(%data: Tensor[(1, 16), float32]) -> Tensor[(1, 32), int32] {
  %0 = (meta[relay.Constant][0] /* ty=Tensor[(32, 16), float32] */, meta[relay.Constant][1] /* ty=Tensor[(32, 16), float32] */);
  %1 = concatenate(%0) /* ty=Tensor[(64, 16), float32] */;
  %2 = nn.dense(%data, %1, units=64) /* ty=Tensor[(1, 64), float32] */;
  %3 = nn.relu(%2) /* ty=Tensor[(1, 64), float32] */;
  %4 = abs(%3) /* ty=Tensor[(1, 64), float32] */;
  %5 = strided_slice(%4, begin=[0, 0], end=[-1, 32], strides=[1, 1], slice_mode="size", axes=None) /* ty=Tensor[(1, 32), float32] */;
  %6 = strided_slice(%4, begin=[0, 32], end=[-1, 32], strides=[1, 1], slice_mode="size", axes=None) /* ty=Tensor[(1, 32), float32] */;
  %7 = add(%5, %6) /* ty=Tensor[(1, 32), float32] */;
  cast(%7, dtype="int32") /* ty=Tensor[(1, 32), int32] */
}
```

![image-20220728212120522](D:\Repo\learning_repo\combine_parallel_dense.assets\image-20220728212120522.png)

这个Pass的主要功能就是将并行的具有相似参数的dense合并成一个dense去进行计算，同时将后面路的op也一起融合。为什么会产生这样的融合策略呢？

在Bert网络中存在QKV三路，实际上可以合并成一个Dense加后续路的操作，这样可以减少op kernel数目，同时一次计算一个较大的kernel也会有一定的性能收益。

其背后执行的Pass 调用的是`ParallelDenseToDenseCombiner(min_num_branches).Combine(expr)`

如果是生成batch_matmul则是调用的另外一个函数了，另外一个Pass结构也是相似，本文只关注一个就行。

所有的类似`combine_parallel_dense`、`combine_parallel_batch_matmul`、`combine_parallel_conv2d`的实现都是继承抽象类`ParallelOpCombine`，实现其主要接口以达成合并操作。

```c++
/*
 * Class to find parallel branches starting with op that are
 * grouped if they are able to be combined. They are eligible to
 * be combined if they have the same input data.
 * Op can be followed by zero or more elemwise or broadcast ops,
 * which are included in the group.
 * Intermediate nodes have exactly one successor. It is possible that branches meet at a point,
 * which should be handled in ParallelOpCombiner.
 *
 *        data
 *       /    \
 *     op      op
 *     |        |
 * elem-wise elem-wise
 *     |        |
 * 这里的op，特指需要合并的op的类型，即某一特定类型的op
 */
class ParallelOpCombiner {
 public:
  // 对外暴露接口，Combine就行了
  Expr Combine(const Expr& expr);

 protected:
  // 判断特定op是否支持合并， 这里的op都是指定特定类型的op
  virtual bool IsSupportedOp(const CallNode* n) = 0;
  // 两个op之间是否支持合并
  virtual bool CanOpsBeCombined(const CallNode* a, const CallNode* b) = 0;
  // 特定类型的op合并为一个新的实现，通常在这之前进行stack或者concat
  virtual Call MakeCombinedOp(const Group& branches) = 0;
  // 特定op后续层之间的参数是否兼容能否合并
  virtual bool IsArgCompatible(const CallNode* a, const CallNode* b, size_t index) = 0;
  // 合并特定op后续层
  virtual Call MakeCombinedCallFromFollowingOps(const Expr& data, const Group& branches,
                                                size_t depth, size_t parent_index) = 0;
  // 更新branch group的输出，主要是为了后续的分支能够兼容合并之后的结果
  virtual void UpdateGroupOutput(const Expr& data, const Group& branches, size_t depth,
                                 ExprSubstMap* subst_map) = 0;

};
```

对于具体实现某一类的op的合并，继承类需要实现上述接口。

总体上合并分支代码的逻辑相对来说清晰

```c++
Expr ParallelOpCombiner::Combine(const Expr& expr) {
  // 找到特定类型Op且可以合并的Op所在分支，
  auto groups = BranchGroupFinder(
                    cached_op_, [&](const CallNode* n) { return IsSupportedOp(n); },
                    [&](const CallNode* a, const CallNode* b) { return CanOpsBeCombined(a, b); })
                    .Find(expr);
  // 遍历分支组的数组，一个group就是满足条件的的多个分支
  for (const Group& group : groups) {
     // 合并需要满足一个最小分支要求，有时候分支过少反而没有收益
    if (group.size() < min_num_branches_) {
      continue;
    }
   //  合并多个分支
    CombineBranches(group);
  }
  // 替换原来表达式中的分支
  return ExprSubst(expr, std::move(subst_map_));
}
```

`BranchGroupFinder`需要你提供三个参数, cached_op指定类型的op、op自身满足的条件判断函数`IsSupportedO`、op和分支的op需要满足的条件判断函数`CanOpsBeCombined`

所以对于合并Dense来说，cached_op当然是`Op::Get("nn.dense")`

```c++
// 只要是dense op 无需确认其条件，都是合格的候选人
bool IsSupportedOp(const CallNode *n) { return true; }

bool CanOpsBeCombined(const CallNode *a, const CallNode *b)
{
    StructuralEqual eq;
    const auto *attrs_a = a->attrs.as<DenseAttrs>();
    const auto *attrs_b = b->attrs.as<DenseAttrs>();
    const auto *weight_a = a->args[1]->type_as<TensorTypeNode>();
    const auto *weight_b = b->args[1]->type_as<TensorTypeNode>();
    ICHECK(attrs_a != nullptr && attrs_b != nullptr && weight_a != nullptr && weight_b != nullptr);
    // 就是正常的比较一下输出的数据类型一直，同时K的维度一致，不过我觉得这里不太严谨，毕竟矩阵可以transpose。比较K维度是否一致，对于矩阵的输出维度N维度，可以不一致，到时候再strideslice的取一下就行
    // output dims (weight->shape[0]) can be different
    return eq(attrs_a->out_dtype, attrs_b->out_dtype) && eq(weight_a->shape[1], weight_b->shape[1]);
}
```

然后对于一个分支来说比较重要的事情就是要将分支如何进行合并， 

```c++
void ParallelOpCombiner::CombineBranches(const Group& branches) {
  // 产生融合的Op dense 三路的权值合并的新dense
  Call combined = MakeCombinedOp(branches);
  auto it = std::min_element(branches.begin(), branches.end(),
                             [](const Branch& branch_a, const Branch& branch_b) {
                               return branch_a.size() < branch_b.size();
                             });
  size_t depth = it->size();
  size_t i;
  // 主要是处理指定op后面的elewise op的融合, 所以起始i = 1
  for (i = 1; i < depth; i++) {
    size_t parent_index;
    // 遍历depth层次的op的输入, 如果输入等于depth - 1层次的op, 说明找到了其在后继节点作为第parent_index个输入
    for (parent_index = 0; parent_index < branches[0][i]->args.size(); parent_index++) {
      if (branches[0][i]->args[parent_index].get() == branches[0][i - 1]) break;
    }
    ICHECK_NE(parent_index, branches[0][i]->args.size());
    // 检查depth层次的op是否可以融合, 同时对于入参的index要一致, 不满足则停止融合后续的op
    if (!CheckLevel(branches, i, parent_index)) break;
    // 将本层的所有op融合, 存在权值则全部在-1维度上全部concat起来，保持入参index的位置的元素一致
    combined = MakeCombinedCallFromFollowingOps(combined, branches, i, parent_index);
  }
  // i = depth, combined = 融合后的所有op, 更新替换map, 主要是给ExprSubst替换后继节点的输出
  UpdateGroupOutput(combined, branches, i - 1, &subst_map_);
}
```

然后就是如何将多路的Dense合并成一个Dense的呢？MakeCombinedOp的重写方法，直接将Dense的权值concat在axis=0的维度上。

```c++
/*
	dense op的输入是 M * K x N * K的矩阵相乘, weight矩阵是转置过的
	d1  M * K x N1 * K = M * N1
	d2  M * K x N2 * K = M * N2
	d3  M * K x N3 * K = M * N3
	合并后的单个dense = M * K x (N1 + N2 + N3) * K的矩阵乘法
	
	
*/

Call MakeCombinedOp(const Group &branches) {
    const Op &dense_op = Op::Get("nn.dense");
    // dense 的两个输入data, weight, 通常来说第二个输入必然是权值, 对于第一个输入是共享输入的, 即公共的输入
    Expr input = branches[0][0]->args[0];
    Expr new_weight;
    IndexExpr new_output_dims;
    // 将weight合并， 即多个weight在axis=0的轴上进行拼接
    std::tie(new_weight, new_output_dims) = TransformWeight(branches);
    const auto *origin_attrs = branches[0][0]->attrs.as<DenseAttrs>();
    ICHECK(origin_attrs);
    // 生成新的DenseAttr以及Op
    const auto dense_attrs = make_object<DenseAttrs>();
    dense_attrs->units = new_output_dims;
    dense_attrs->out_dtype = origin_attrs->out_dtype;
    return Call(dense_op, {input, new_weight}, Attrs{dense_attrs}, {});
}
```

判断dense后续的分支op是否参数兼容， 这里主要由一点就是原来的三路输出是 M * N1, M * N2, M * N3

```c++
bool IsArgCompatible(const CallNode *a, const CallNode *b, size_t index)
{
    StructuralEqual eq;
    auto ta = a->args[index]->type_as<TensorTypeNode>();
    auto tb = b->args[index]->type_as<TensorTypeNode>();
    auto toutput_a = a->type_as<TensorTypeNode>();
    auto toutput_b = b->type_as<TensorTypeNode>();
    ICHECK(ta != nullptr && tb != nullptr && toutput_a != nullptr && toutput_b != nullptr);

    if (!eq(ta->dtype, tb->dtype) || ta->shape.size() != tb->shape.size())
    {
        return false;
    }
    if (toutput_a->shape.size() < ta->shape.size() || toutput_b->shape.size() < tb->shape.size())
    {
        return false; // not broadcast/elemwise
    }
    if (ta->shape.size() > 0)
    {
        for (size_t i = 0; i < ta->shape.size() - 1; i++)
        {
            // shape dims must match except last dim
            if (!eq(ta->shape[i], tb->shape[i]))
                return false;
        }
    }
    return true;
}
```

