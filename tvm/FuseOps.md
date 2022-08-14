# FuseOps

FuseOps主要是将多个op融合在一起，产生的效果为在后端实际编译过程中，作为一个kernel执行，这样的话，层与层之间的数据仍然留在寄存器，减少寄存器拷贝到device内存，然后再从device内存拷贝到寄存器一个round的。

```c++
// 融合前mod
def @main /* id=114364256 */(%data: Tensor[(10), float32]) -> Tensor[(10), float32] {
  %0 = divide(%data, meta[relay.Constant][0] /* ty=Tensor[(10), float32] */) /* ty=Tensor[(10), float32] */;
  %1 = multiply(%0, 2f /* ty=float32 */) /* ty=Tensor[(10), float32] */;
  nn.relu(%1) /* ty=Tensor[(10), float32] */
}

// 融合后mod
def @main /* id=114364256 */(%data: Tensor[(10), float32]) -> Tensor[(10), float32] {
  %2 = fn (%p0: Tensor[(10), float32], %p1: Tensor[(10), float32], Primitive=1) -> Tensor[(10), float32] {
    %0 = divide(%p0, %p1) /* ty=Tensor[(10), float32] */;
    %1 = multiply(%0, 2f /* ty=float32 */) /* ty=Tensor[(10), float32] */;
    nn.relu(%1) /* ty=Tensor[(10), float32] */
  };
  %2(%data, meta[relay.Constant][0] /* ty=Tensor[(10), float32] */) /* ty=Tensor[(10), float32] */
}
```

其主体流程为，其主体流程还是比较清晰的，创建IndexForwardGraph，然后对图根据fuse_opt_level和max_fuse_depth进行分组，最后改变。

```c++
Expr Transform(const Expr &body, int fuse_opt_level, size_t max_fuse_depth) {
    // setup the group map.
    auto graph = IndexedForwardGraph::Create(&arena_, body);
    auto groups = GraphPartitioner(&arena_, fuse_opt_level, max_fuse_depth).Partition(graph);
    for (size_t nid = 0; nid < graph.post_dfs_order.size(); ++nid) {
        ICHECK(graph.post_dfs_order[nid]->ref != nullptr);
        gmap_[graph.post_dfs_order[nid]->ref] = groups[nid];
    }
    // The following line can be used for debug.
    // this->DebugDumpGroup(body);
    return this->Mutate(body);
}


/*!
 * \brief Indexed data flow graph in forward direction.
 *  This is a temporary data structure used for operator fusion analysis.
 *
 *  This data structure only captures the dataflow fragment and
 *  could ignore blocks like let by simply ordering each dataflow block
 *  and mark the output node as extern_ref;
 */
class IndexedForwardGraph {
public:
    struct Node;
    /*!
     * The forward edge in the dataflow graph.
     */
    struct Edge {
        /*! \brief The corresponding node */
        Node *node{nullptr};
        /*! \brief The respective pattern of this op */
        OpPatternKind pattern{kOpaque};
    };
    /*! \brief A node in the graph. */
    struct Node {
        /*! \brief weak reference to the corresponding edge. */
        const tvm::Object *ref{nullptr};
        /*! \brief The index of the node in topological order. */
        size_t index{0};
        /*! \brief Whether this node is referenced by external source */
        bool extern_ref{false};
        /*! \brief The general pattern in the node */
        OpPatternKind pattern{kOpaque};
        /*! \brief The outputs of the node. */
        LinkedList<Edge> outputs;
    };
    /*! \brief The node map that maps node to graph */
    std::unordered_map<const tvm::Object *, Node *> node_map;
    /*! \brief All the nodes in post DFS order */
    std::vector<Node *> post_dfs_order;
};

IndexedForwardGraph Prepare(const Expr &body) {
    this->Update(body, nullptr, kOpaque);
    this->VisitExpr(body);
    return std::move(graph_);
}
```

`IndexedForwardGraph` DAG图，使用边将节点连接起来可以看到每个节点都存在outputs, outputs里面是链表存的`Edge`，而Edge的数据结构，则是对应输出的对端Node。同时每个节点都存在topological order的索引。

这里要介绍的一个概念就是topo排序，在图论中，由一个有向无环图的顶点组成的序列，当且仅当满足下列条件时，称为该图的一个拓扑排序（英语：Topological sorting）。

- 每个顶点出现且只出现一次
- 若A在序列中排在B的前面，则在图中不存在从B到A的路径。

其中存在一个Update方法，首先将最后的节点标记为无输出。

```c++
// 主要以更新后继节点为主， 主要是建立node和后继节点之间的关系，以及连接方式。
void Update(const Expr &node, IndexedForwardGraph::Node *parent, OpPatternKind pattern) {
    // parent 为后继节点
    const tvm::Object *key = node.get();
    IndexedForwardGraph::Node *current;
    auto it = graph_.node_map.find(key);
    // 从node_map中查找是否遍历过，遍历过直接找到节点
    if (it != graph_.node_map.end()) {
        current = it->second;
    } else {
        current = arena_->make<IndexedForwardGraph::Node>();
        graph_.node_map[key] = current;
    }
    // 如果后继节点存在
    if (parent != nullptr) {
        // 添加输出边，输出遍历边连接后继节点
        auto *link = arena_->make<LinkNode<IndexedForwardGraph::Edge>>();
        link->value.node = parent;
        link->value.pattern = pattern;
        current->outputs.Push(link);
    } else {
        // 无后继节点，设置为输出
        current->extern_ref = true;
    }
}

// 在AddNode方法中，主要是为了更新post_dfs_order图，在之前的Update操作中，节点已经存在于node_map中，这个时候将node和实际ExprNode对应联系起来。
// 根据post_order_dfs到的顺序打上标签。即index 为0的一定是最早遍历到的
void AddNode(const tvm::Object *key) {
    auto it = graph_.node_map.find(key);
    ICHECK(it != graph_.node_map.end()) << "Cannot find node " << GetRef<ObjectRef>(key);
    IndexedForwardGraph::Node *node = it->second;
    ICHECK(node->ref == nullptr);
    node->ref = key;
    node->index = graph_.post_dfs_order.size();
    graph_.post_dfs_order.push_back(node);
}
```

`IndexedForwardGraph` 对所有的Node类型都重载了`VisitExpr_`

以一个简单的case为例, divide, multiply, relu

```c++
fn (%data: Tensor[(1, 3, 4, 4), float32]) -> Tensor[(1, 3, 2, 2), float32] {
  %0 = divide(%data, meta[relay.Constant][0] /* ty=Tensor[(1, 3, 4, 4), float32] */) /* ty=Tensor[(1, 3, 4, 4), float32] */;
  %1 = multiply(%0, 2f /* ty=float32 */) /* ty=Tensor[(1, 3, 4, 4), float32] */;
  %2 = nn.relu(%1) /* ty=Tensor[(1, 3, 4, 4), float32] */;
  %3 = nn.max_pool2d(%2, pool_size=[2, 2], padding=[0, 0, 0, 0]) /* ty=Tensor[(1, 3, 3, 3), float32] */;
  %4 = nn.relu(%3) /* ty=Tensor[(1, 3, 3, 3), float32] */;
  %5 = nn.max_pool2d(%4, pool_size=[2, 2], padding=[0, 0, 0, 0]) /* ty=Tensor[(1, 3, 2, 2), float32] */;
  nn.relu(%5) /* ty=Tensor[(1, 3, 2, 2), float32] */
}
```

根据这个图，我们大概可以如下的关系图， 这是只有1个入参的函数，

首先进来会遍历到`FunctionNode`

```c++
void VisitExpr_(const FunctionNode *op) final {
    // 函数是kCompiler类型的交给后端编译器去处理
    if (op->GetAttr<String>(attr::kCompiler).defined())
        return;
	// 将op的入参全部更新到node-map, 并且没有后继节点，因为当前还没遍历到，所以这部分关系还不太明确
    for (auto param : op->params)
    {
        this->Update(param, nullptr, kOpaque);
    }
    // 当op的body部分也添加进去
    this->Update(op->body, nullptr, kOpaque);
    ExprVisitor::VisitExpr_(op);
}
```

然后调用`ExprVisitor`的方法对params和body进行遍历访问，首先编译到的是名为data的VarNode即Function的params，访问到VarNode直接添加索引index 是什么时候访问的。

```c++
void VisitExpr_(const VarNode* op) final {
    this->AddNode(op);
}
```

在这个之后Var便利完， 这里需要特别说明是kOpaque则必然是不会进行融合的Op

```c++
void VisitExpr_(const CallNode* call) final {
    ICHECK(graph_.node_map.count(call));
    Node* node = graph_.node_map.at(call);
    static auto fpattern = Op::GetAttrMap<TOpPattern>("TOpPattern");
    // 对于OpNode使用注册的OpPattern，没有注册使用kOpaque. 如果不是OpNode，说明是个函数，则更新节点，注册为kOpaque。
    // 动态op和数据依赖op默认为kOpaque
    OpPatternKind op_pattern = kOpaque;
    if (const OpNode* opnode = call->op.as<OpNode>()) {
      auto op = GetRef<Op>(opnode);
      if (IsDynamic(call->checked_type()) && IsDataDependent(call)) {
        op_pattern = kOpaque;
      } else {
        op_pattern = static_cast<OpPatternKind>(fpattern[op]);
      }
    } else {
      this->Update(call->op, node, kOpaque);
    }

    node->pattern = op_pattern;
    this->Update(call->op, nullptr, kOpaque);
    const auto* rtype = call->checked_type().as<TensorTypeNode>();
    for (size_t i = 0; i < call->args.size(); ++i) {
      const auto* arg_type = call->args[i]->checked_type().as<TensorTypeNode>();
      // 在本身Op是braodcast的情况下，但是没有发生broadcast，即输入输出shape没有变，edge的属性则调整为EleWise
      OpPatternKind edge_pattern = op_pattern;
      if (edge_pattern == kBroadcast && arg_type != nullptr && rtype != nullptr &&
          attr_equal_(rtype->shape, arg_type->shape)) {
        edge_pattern = kElemWise;
      }
      this->Update(call->args[i], node, edge_pattern);
    }
    // 使用父类的去访问CallNode的属性节点，对于CallNode主要是访问输入，然后是body
    ExprVisitor::VisitExpr_(call);
    this->AddNode(call); // 添加节点CallNode
}
```

在遍历完之后就是得到一个可以DebugDump输出

```c++
node[0], Var(data, ty=TensorType([1, 3, 4, 4], float32)) outputs=[2:0, ]
node[1], Constant([...]) outputs=[2:0, ]
node[2], CallNode(Op(divide), [Var(data, ty=TensorType([1, 3, 4, 4], float32)), Constant([...])], (nullptr), [TensorType([1, 3, 4, 4], float32), TensorType([1, 3, 4, 4], float32)]) outputs=[4:0, ]
node[3], Constant(2.0) outputs=[4:1, ]
node[4], CallNode(Op(multiply), [CallNode(Op(divide), [Var(data, ty=TensorType([1, 3, 4, 4], float32)), Constant([...])], (nullptr), [TensorType([1, 3, 4, 4], float32), TensorType([1, 3, 4, 4], float32)]), Constant(2.0)], (nullptr), [TensorType([1, 3, 4, 4], float32), TensorType([], float32)]) outputs=[5:0, ]
node[5], CallNode(Op(nn.relu), [CallNode(Op(multiply), [CallNode(Op(divide), [Var(data, ty=TensorType([1, 3, 4, 4], float32)), Constant([...)], (nullptr), [TensorType([1, 3, 4, 4], float32), TensorType([1, 3, 4, 4], float32)]), Constant(2.0)], (nullptr), [TensorType([1, 3, 4, 4], float32), TensorType([], float32)])], (nullptr), [TensorType([1, 3, 4, 4], float32)]) outputs=[6:4, ]
node[6], CallNode(Op(nn.max_pool2d), [CallNode(Op(nn.relu), [CallNode(Op(multiply), [CallNode(Op(divide), [Var(data, ty=TensorType([1, 3, 4, 4], float32)), Constant([...)], (nullptr), [TensorType([1, 3, 4, 4], float32), TensorType([1, 3, 4, 4], float32)]), Constant(2.0)], (nullptr), [TensorType([1, 3, 4, 4], float32), TensorType([], float32)])], (nullptr), [TensorType([1, 3, 4, 4], float32)])], relay.attrs.MaxPool2DAttrs(0x61a8fa8), [TensorType([1, 3, 4, 4], float32)]) outputs=[7:0, ]
node[7], CallNode(Op(nn.relu), [CallNode(Op(nn.max_pool2d), [CallNode(Op(nn.relu), [CallNode(Op(multiply), [CallNode(Op(divide), [Var(data, ty=TensorType([1, 3, 4, 4], float32)), Constant([...)], (nullptr), [TensorType([1, 3, 4, 4], float32), TensorType([1, 3, 4, 4], float32)]), Constant(2.0)], (nullptr), [TensorType([1, 3, 4, 4], float32), TensorType([], float32)])], (nullptr), [TensorType([1, 3, 4, 4], float32)])], relay.attrs.MaxPool2DAttrs(0x61a8fa8), [TensorType([1, 3, 4, 4], float32)])], (nullptr), [TensorType([1, 3, 3, 3], float32)]) outputs=[8:4, ]
node[8], CallNode(Op(nn.max_pool2d), [CallNode(Op(nn.relu), [CallNode(Op(nn.max_pool2d), [CallNode(Op(nn.relu), [CallNode(Op(multiply), [CallNode(Op(divide), [Var(data, ty=TensorType([1, 3, 4, 4], float32)), Constant([...)], (nullptr), [TensorType([1, 3, 4, 4], float32), TensorType([1, 3, 4, 4], float32)]), Constant(2.0)], (nullptr), [TensorType([1, 3, 4, 4], float32), TensorType([], float32)])], (nullptr), [TensorType([1, 3, 4, 4], float32)])], relay.attrs.MaxPool2DAttrs(0x61a8fa8), [TensorType([1, 3, 4, 4], float32)])], (nullptr), [TensorType([1, 3, 3, 3], float32)])], relay.attrs.MaxPool2DAttrs(0x61280f8), [TensorType([1, 3, 3, 3], float32)]) outputs=[9:0, ]
node[9], CallNode(Op(nn.relu), [CallNode(Op(nn.max_pool2d), [CallNode(Op(nn.relu), [CallNode(Op(nn.max_pool2d), [CallNode(Op(nn.relu), [CallNode(Op(multiply), [CallNode(Op(divide), [Var(data, ty=TensorType([1, 3, 4, 4], float32)), Constant([...)], (nullptr), [TensorType([1, 3, 4, 4], float32), TensorType([1, 3, 4, 4], float32)]), Constant(2.0)], (nullptr), [TensorType([1, 3, 4, 4], float32), TensorType([], float32)])], (nullptr), [TensorType([1, 3, 4, 4], float32)])], relay.attrs.MaxPool2DAttrs(0x61a8fa8), [TensorType([1, 3, 4, 4], float32)])], (nullptr), [TensorType([1, 3, 3, 3], float32)])], relay.attrs.MaxPool2DAttrs(0x61280f8), [TensorType([1, 3, 3, 3], float32)])], (nullptr), [TensorType([1, 3, 2, 2], float32)]) outputs=[]
```

这里新增了修改了DebugDump，使得DebugDump能够打印出节点的后续节点连接的op类型，在上图节点输出为0根据下面的映射关系可以都是ElemWise节点的出。

```
/*! \brief operator pattern used in graph fusion */
enum OpPatternKind {
  // Elementwise operation
  kElemWise = 0,
  // Broadcasting operator, can always map output axis to the input in order.
  // for example :code:`out[i, ax1, j, ax2] = input[i, j]`.
  // Note that the axis need to be in order so transpose is not a bcast operator.
  kBroadcast = 1,
  // Injective operator, can always injectively map output axis to a single input axis.
  // All injective operator can still be safely fused to injective and reduction.
  kInjective = 2,
  // Communicative reduction operator.
  kCommReduce = 3,
  // Complex operation, can still fuse elemwise operations into its output.
  // but cannot chain another complex op
  kOutEWiseFusable = 4,
  // The pattern for tuple nodes. Can fuse into subsequent injective ops,
  // but treated specially
  kTuple = 7,
  // Opaque operation, cannot fuse anything.
  kOpaque = 8
};
```

可以看到节点的node[0] var 和node[1] const输出给了2

节点[2, 3]的输出给了4。同时对于relu的输出后面接的后继节点的oppattern 为4，即这个节点后面的可以接elewise op.

在创建IndexedGraph的过程中

```c++
#[version = "0.0.5"]
fn (%data: Tensor[(1, 3, 4, 4), float32] /* group=0x61dc348 */) -> Tensor[(1, 3, 2, 2), float32] {
  %0 = divide(%data, meta[relay.Constant][0] /* group=0x61dc370 */) /* group=0x61dc410 */;
  %1 = multiply(%0, 2f /* group=0x61dc410 */) /* group=0x61dc410 */;
  %2 = nn.relu(%1) /* group=0x61dc410 */;
  %3 = nn.max_pool2d(%2, pool_size=[2, 2], padding=[0, 0, 0, 0]) /* group=0x61dc460 */;
  %4 = nn.relu(%3) /* group=0x61dc460 */;
  %5 = nn.max_pool2d(%4, pool_size=[2, 2], padding=[0, 0, 0, 0]) /* group=0x61dc4b0 */;
  nn.relu(%5) /* group=0x61dc4b0 */
}
```

可以看到上面的图产生了5个group，而var和const 都分别在两个group中，剩下的function计算则是分为了3个group。看一下分组的主要逻辑

```c++
std::vector<GraphPartitioner::Group*> GraphPartitioner::Partition(
    const IndexedForwardGraph& graph) {
  this->InitGroups(graph);
  if (opt_level_ == 0) return std::move(groups_);
  // get post dominator tree
  auto post_dom_tree = DominatorTree::PostDom(arena_, graph);
  // run fusion algorithm.
  for (int phase = 0; phase < 3; ++phase) {
    this->RunFuse(graph, post_dom_tree, phase);
  }
  return std::move(groups_);
}
```

`InitGroups` 将之前生成的IndexedGraph每一个都放到一个Group中，每个Group的属性都和Node的属性一致，如果opt_level 为0，那么就不进行融合。所以这是一个Level 1的Pass

然后就是PostDom





[MLC Course](https://mlc.ai/chapter_introduction/index.html)
