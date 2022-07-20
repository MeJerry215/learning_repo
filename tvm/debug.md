# TVM Debug

[TOC]

TVM Debug手段总结，TVM相对来说Debug比较困难一点。这里将会记录所有遇到的一些问题以期望能够遇到相似的问题能够找到一些思路。

## Debug Method

Q1： 如果能使用Python来打印一些东西，为什么要在cpp文件中手写打印呢？ gdb有时候打印不出来具体的数据结构，但是如果使用pdb是不是会更简单一点呢？

A1：可以在需要debug的地方插入`RELAY_DEBUG`宏，将你需要打印的参数作为参数传入。由于ffi的存在，c++中的任意Object都能在python中获取并打印。

```python
@tvm._ffi.register_func("relay.debug")
def _debug(*args):
    import pdb

    pdb.set_trace()
```

args 第三个之后就是你传入的参数，前面三个参数为固定的"RELAY_DEBUG"、文件名、行数，后续才是你的参数。

```c++
#define RELAY_DEBUG(...)                                                \
  {                                                                     \
    auto fdebug = runtime::Registry::Get("relay.debug");                \
    ICHECK(fdebug) << "Could not find Relay Python debugger function."; \
    (*fdebug)("RELAY_DEBUG", __FILE__, __LINE__, __VA_ARGS__);          \
  }
```

![image-20220720160846050](D:\Repo\learning_repo\tvm\debug.assets\image-20220720160846050.png)

这里简单举例，实际这边用直接打印会更加快，但是对于复杂的数据结构来说，可以考虑一下使用Python Debug.

当执行到此处时，就会调用到Python函数，

```shell
(Pdb) p args[3].opt_level
0
(Pdb) p args[3].name
'InferType'
(Pdb) p args[3].required
[]
(Pdb) type(args[3])
<class 'tvm.ir.transform.PassInfo'>
(Pdb) 
```

