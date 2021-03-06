# Cpp Learning
[TOC]

**左值和右值**

左值: *are-value*, 左值可以出现在**赋值语句的左侧**，左值表达式的结果是一个对象或者是函数，用的是对象的身份。 左值持久。
右值: *ell-value*, 出现在赋值语句的右侧，当对象作为右值时没用的是对象的值。右值短暂。

使用右值得时候可以使用左值代替，反之不行。

右值引用: 只能绑定到即将销毁的的临时对象，该对象没有其他用户。


**类型转换**
static_cast、dynamic_cast、const_cast、reinterpret_cast， 4种。

static_cast 等价于隐式转换。

const_cast 去掉底层const。

reinterpret_cast  编译器按照目标转换类型在内存中所占的比特位重新编译原始数据类型。转换前后对象的地址值是不变的，只是对象的读取内存的比例变了而已。

dynamic_cast是将一个基类对象指针（或引用）转换到继承类指针，dynamic_cast会根据基类指针是否真正指向继承类指针来做相应处理。

**不要返回局部对象的引用或指针，可是返回引用左值**


**函数重载(overload)、函数覆盖(override)、函数重写(overwrite)**

重载针对于函数名相同，但形参列表、类型不同, 存在const版本调用const版本

覆盖针对函数名相同，形参列表相同，带virtual，即多态

函数隐藏函数名相同，形参列表相同.

## 动态内存

new, delete, shared_ptr, unique_ptr, weak_ptr

shared_ptr: 多个指针指向同一个对象

unique_ptr: 独占所指向的对象，

用unique_ptr初始化shared_ptr，内存控制权发生转移。 unique_ptr不能拷贝，存在例外即将要销毁的unique_ptr可以拷贝。

weak_ptr 指向share_ptr管理的对象

allocator 将内存分配和构造分离，分配内存之后，然后使用allocator构造对象。

## 顺序容器

vector, deque, list, forward_list, array, string.

**capacity, reserve, resize**: capacity在不扩张内存空间的情况下可以容纳多少个元素, reserve容器应该保存多少个元素, 当需要内存超过当前capacity时改变容量.
size表示已经保存的元素得数目。

容器适配器: stack, queue, priority_queue。默认情况下，stack和queue基于deque实现，priority_queue是在vector之上实现。

## 关联容器

map, set, multimap, multiset, unordered_map, unordered_set, unorder_multismap, unorder_multiset


## 泛型算法

泛型算法基于迭代器之上，而不执行容器操作相关特定，算法不会改变叠层容器的大小，但是可能改变容器的保存元素值。


## 迭代器

插入迭代器、流迭代器、反向迭代器、移动迭代器

插入迭代器：`back_inserter`、`front_inserter`、 `inserter`

流迭代器: `istream_iterator`

反向迭代器: ``
## 类

构造、拷贝、赋值、销毁： 拷贝构造函数，拷贝赋值运算、移动构造函数、移动赋值运算符号、析构函数。

拷贝和移动构造函数定义了当使用同类型另一个对象初始化本对象时做什么。

拷贝和移动赋值定义了将一个对同类型的对象赋值时候做什么。

析构定义了对象销毁时候做什么。

拷贝初始化使用时机:
- 使用`=`定义变量时发生
- 将一个对象作为实参传递给一个非引用类型形参
- 返回值非引用类型返回一个对象
- 使用花括号初始化
[cpp 接口查询](https://en.cppreference.com/w/cpp/utility/tuple/tie)

## OOP

https://www.geeksforgeeks.org/template-specialization-c/?ref=lbp

