# Relay Op

Relay层Op仅仅代表了一个占位符，在计算图中标注这里存在一个名字叫什么什么的op，有的时候甚至op会被Realize成小的op，在后续的流程中根据注册的strategy选择对应的实现，也就是lower。

在理解relay的时候首先要知道一点就是`Expr`和`ExprNode`， relay中的Op都继承自以上两个类。`Expr`作为接口封装，作为一个数据持有者，而持有的就是`ExprNode`。

`Expr`继承自`BaseExpr`，再往上一层的继承`ObjectRef`提供了智能指针的功能，让让子类能够访问实际数据。同样的`ExprNode`继承自`BaseExprNode`, `BaseExprNode`继承自`Object`。

注意这样的继承对应关系以及对象关系。在relay ir中，访问的是`Expr`， 通过`Expr`访问数据`ExprNode`。

举个简单的例子`Constant`和`ConstantNode`。我们知道常量是一个非常重要的节点在我们在使用`relay.const`

实际上是用过FFI调用到了注册构造函数

```c++
Constant::Constant(runtime::NDArray data, Span span) {
  ObjectPtr<ConstantNode> n = make_object<ConstantNode>();
  n->data = std::move(data);
  n->span = std::move(span);
  data_ = std::move(n);
}

TVM_REGISTER_NODE_TYPE(ConstantNode);

TVM_REGISTER_GLOBAL("relay.ir.Constant").set_body_typed([](runtime::NDArray data) {
  return Constant(data);
});
```

这是一个删减了部分细节的`Constant`和`ConstantNode`的定义

```c++
class ConstantNode : public ExprNode {
 public:
  runtime::NDArray data;
  ...
  static constexpr const char* _type_key = "relay.Constant";
  TVM_DECLARE_FINAL_OBJECT_INFO(ConstantNode, ExprNode);
};

class Constant : public Expr {
 public:
  TVM_DLL explicit Constant(runtime::NDArray data, Span span = Span());
  ...
  TVM_DEFINE_OBJECT_REF_METHODS(Constant, RelayExpr, ConstantNode);
};
```

`Constant`除了将用data构造`ConstantNode`没有其他的方法，而所有的参数都是存在于`ConstantNode`之中。

如果要通过`Constant`访问其数据部分，使用其继承自父类`ObjectRef`的方法。

```c++
Constant constant;
Expr expr = constant;

// 得到是向上转型后的expr： Expr, 使用这种方法比较安全，如果不能转型到则会返回到null
const auto* constant = expr.as<ConstantNode>();
constant->data;
// 得到的是Constant
constant.get()->data;
constant->data;
```

Relay中的基础数据类型都定义在`include/tvm/relay/expr.h`中，作为常用的数据类型大致可以分为

- Constant：常量封装，表示该节点是一个常量节点，通常用来存放权值或者一些节点的参数。
- Tuple： 元组，Tensor数组，对于一个节点存在多个输出，其输出就是一个Tuple，典型的比如`batch_norm`
- TupleGetItem：对于Tuple中存在的多个数据，访问其中某个数据就要使用这个作为封装。
- Var：变量，变量对应的是一个输入，经常在某些计算Op，其输入可以是Var 或者是Constant类型的，输入Var在后期也能通过bind方式，将`Var`和`Constant`进行绑定。
- Call：函数调用op，表示进行了一次函数调用，调用的op就是其内部持有的op。
- Let：赋值op，将某个Expr的结果赋值给一个新的变量
- If：条件判断op，内部存在condition，true_branch, false_brach。用来表示一个条件语句。
- RefCreate：创建对某个Tensor的引用，即起了个别名类似的概念，后续可以通过调用RefRead或者RefWriter对其进行操作.
- RefRead: ...
- RefWrite：...
- TempExpr：中间形态的Expr，通常不会存在实际的计算op对应，在做图优化或者转换的时候需要这样的op，然后对这些op替换realize。

所有的图中有意义的op都是CallOp的一次封装，理解起来可能很难理解，但是如果我们举例例子比如bias_add调用实际上调用的是

```c++
Expr MakeBiasAdd(Expr data, Expr bias, int axis) {
  auto attrs = make_object<BiasAddAttrs>();
  attrs->axis = axis;
  static const Op& op = Op::Get("nn.bias_add");
  return Call(op, {data, bias}, Attrs(attrs), {});
}
```

你可以理解为如果我调用了`nn.bias_add(x, y)`，就是一次对`Op("nn.bias_add")`的一次`Call`操作，所有的实体意义Op最后都会转换为`Op`类以及其对应的`Attrs`。这里就引入了类`Op`和`OpNode`的概念。Op也是继承自`Expr`，持有`ExprNode`和上述的基础数据类型保持一样的继承结构。

```c++
class Op : public RelayExpr {
 public:
  explicit Op(ObjectPtr<Object> n) : RelayExpr(n) {}
  template <typename ValueType>
  TVM_DLL static const Op& Get(const String& op_name);

  using ContainerType = OpNode;

};

class OpNode : public RelayExprNode {
 public:
  String name;
  mutable FuncType op_type;
  String description;
  Array<AttrFieldInfo> arguments;
  String attrs_type_key;
  uint32_t attrs_type_index{0};
  int32_t num_inputs = -1;

  int32_t support_level = 10;

  static constexpr const char* _type_key = "Op";
  TVM_DECLARE_FINAL_OBJECT_INFO(OpNode, RelayExprNode);
};
```

在了解以上内容之后，OpNode描述了本身的输入、输出、类型推理、属性类型、计算方式。再理解Op注册就比较容易了。

要注册上1个包含属性节点的Op通常包含如下4步骤：

1. 添加类BiasAddAttrs，并注册上属性

    ```c++
    struct BiasAddAttrs : public tvm::AttrsNode<BiasAddAttrs> {
      int axis;

      TVM_DECLARE_ATTRS(BiasAddAttrs, "relay.attrs.BiasAddAttrs") {
        TVM_ATTR_FIELD(axis).describe("The axis to add the bias").set_default(1);
      }
    };

    TVM_REGISTER_NODE_TYPE(BiasAddAttrs);
    ```

2. 注册Op BiasAdd，描述其基本特征

   ```c++
   RELAY_REGISTER_OP("nn.bias_add")
       .describe(R"code(Add bias to an axis of the input.
   
   )code" TVM_ADD_FILELINE)
       .set_attrs_type<BiasAddAttrs>()
       .set_num_inputs(2)
       .add_argument("data", "nD Tensor", "Input data.")
       .add_argument("bias", "1D Tensor", "Bias.")
       .set_support_level(1)
       .add_type_rel("BiasAdd", BiasAddRel)
       .set_attr<FTVMCompute>("FTVMCompute", [](const Attrs& attrs, const Array<te::Tensor>& inputs,
                                                const Type& out_type) {
         const auto* param = attrs.as<BiasAddAttrs>();
         return tvm::Array<tvm::te::Tensor>{topi::nn::bias_add(inputs[0], inputs[1], param->axis)};
       });
   ```
   
   bias_add 的属性类型是BiasAddAttrs，存在两个输入，其中1个输入叫做data，另外一个叫bias。同时注册上op的类型推测函数(BiasAddRel)，Op计算方式。
   
3. 注册Op构造函数，主要是为了给Python层能够调用到。
   
   ```
   Expr MakeBiasAdd(Expr data, Expr bias, int axis) {
     auto attrs = make_object<BiasAddAttrs>();
     attrs->axis = axis;
     static const Op& op = Op::Get("nn.bias_add");
     return Call(op, {data, bias}, Attrs(attrs), {});
   }
   
   TVM_REGISTER_GLOBAL("relay.op.nn._make.bias_add").set_body_typed(MakeBiasAdd);
   ```
   
   从Op构造函数一般化到其他网络中的op，都是使用Call 封装了实际Op节点，Op抽象了节点，抽象成几个输入几个输出、输入什么shape、输出什么shape、op名称、计算方式。而Op的一些参数实体信息则是绑定到Call上面
   
4. 提供上层python接口
   
   ```python
   def bias_add(data, bias, axis=1):
       return _make.bias_add(data, bias, axis)
   ```
   
   
   
   有的朋友可能会好奇，如果我的一个Op存在多个输出，属性里面也没有描述，改如何实现。
   
   首先你的Compute 方法就在return的时候的时候，输出的结果是多个，其次在python层接口使用TupleWrapper描述Op存在几个输出。
   
   这里比较典型的就是batch_norm

   ````python
    def batch_norm(
        data, gamma, beta, moving_mean, moving_var, axis=1, epsilon=1e-5, center=True, scale=True
    ):
        result = _make.batch_norm(
            data, gamma, beta, moving_mean, moving_var, axis, epsilon, center, scale
        )
        return expr.TupleWrapper(result, 3)
    ```
   ````
   
   batch_norm的输出结果直接用TupleWrapper封装，描述存在3个输出



这里推荐两个可视化工具，一个是

github的官方提交的可视化工具

https://github.com/apache/tvm/commit/55cfc4ad2df0120411b6b5ae2a2f28d8b467a25a#diff-ffafde7b705a970be1cee1482b34bf75960d1b402c1cafa186344885ecb77f74

https://xinetzone.github.io/tvm-book/tutorials/ir/relay_viz.html

```python
data = relay.var("data")
bias = relay.var("bias")
add_op = relay.add(data, bias)
add_func = relay.Function([data, bias], add_op)
add_gvar = relay.GlobalVar("AddFunc")

input0 = relay.var("input0")
input1 = relay.var("input1")
input2 = relay.var("input2")
add_01 = relay.Call(add_gvar, [input0, input1])
add_012 = relay.Call(add_gvar, [input2, add_01])
main_func = relay.Function([input0, input1, input2], add_012)
main_gvar = relay.GlobalVar("main")

mod = tvm.IRModule({main_gvar: main_func, add_gvar: add_func})

######################################################################
# Render the graph with Relay Visualizer on the terminal
# ------------------------------------------------------
# The terminal can show a Relay IR module in text similar to clang AST-dump.
# We should see ``main`` and ``AddFunc`` function. ``AddFunc`` is called twice in the ``main`` function.
viz = relay_viz.RelayVisualizer(mod)
viz.render()

viz = RelayVisualizer(mod, plotter=DotPlotter())
viz.render("add")
```



这个工具则从图的构图的基础上描述了图

![image-20220720144839491](D:\Repo\learning_repo\tvm\relay_op.assets\image-20220720144839491.png)

CallNode的op部分就是函数的计算函数，可以是expr 也是是function，其后紧接着的是args，这里并没有打印出属性节点。

在上图中GlobalVar AddFunc 就是op， `Var(Input) name_hint:input1` 和`Var(Input) name_hint:input2` 就是图的args

另外一个是

知乎博主 Archer 文章[【我与TVM二三事 前篇（2）】relay optimize准备知识](https://zhuanlan.zhihu.com/p/457131758) 中提到的工具

https://github.com/Archermmt/tvm_walk_through/blob/master/visualize.py

这个工具相对更加关注op之间的关系

![image-20220720144808982](D:\Repo\learning_repo\tvm\relay_op.assets\image-20220720144808982.png)

