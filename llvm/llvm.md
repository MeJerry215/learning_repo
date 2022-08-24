# LLVM

[LLVM Introduction](http://www.aosabook.org/en/llvm.html)

[LLVM Programmer's Mannual](https://llvm.org/docs/ProgrammersManual.html)



[Git Version Control Cookbook](https://www.packtpub.com/product/git-version-control-cookbook/9781782168454)

[Learn-LLVM-12-A-beginners-guide-to-learning-LLVM-compiler-tools-and-core-libraries-with-Cpp-2021](Learn-LLVM-12-A-beginners-guide-to-learning-LLVM-compiler-tools-and-core-libraries-with-Cpp-2021.pdf)

LLVM架构，主要有前端、优化器、和后端组成。前端解析代码，检查语法错误，然后生成语法树。优化器对于生成的语法树进行优化，优化后的语法树送给后端。

![[Three Major Components of a Three-Phase Compiler]](D:\Repo\learning_repo\llvm\llvm.assets\SimpleCompiler.png)

进一步说，可以使用下图表示具体代码到不同后端

![[Retargetablity]](D:\Repo\learning_repo\llvm\llvm.assets\RetargetableCompiler.png)

通过这种架构，编译器可以支持新的编程语言，只要实现新的前端。现有的优化器和后端可以复用复用。

编译

```
cmake -DLLVM_ENABLE_PROJECTS=clang –DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_TARGETS_TO_BUILD="X86" ../llvm -DBUILD_SHARED_LIBS=ON
```



**目录架构**

- llvm/cmake/modules： 编译配置
- llvm/cmake/platforms： 平台工具链配置
- llvm/examples： 例子
- llvm/include/llvm： LLVM所有代码，主要是: Analysis, Codegen, Target, Transforms, etc...
- llvm/include/llvm/Support：C++ STL工具函数以及命令行解析头文件
- llvm/include/llvm/Config：cmake产生配置文件
- llvm/lib/IR：llvm核心库
- llvm/lib/AsmParser：汇编语言解析库
- llvm/lib/Bitcode：读写bitcode
- llvm/lib/Analysis：程序分析器
- llvm/lib/Transforms： IR-to-IR转换器，比如dead code消除
- llvm/lib/Target：目标平台架构描述
- llvm/lib/Codegen：代码生成
- llvm/lib/MC： 机器码处理，主要处理汇编和object文件产生
- llvm/lib/ExecutionEngine：runtime和JIT
- llvm/lib/Support：...
- llvm/tools：llvm工具，比如llvm-as, llvm-dis, llvm-link, lli, llc, opt

```c++
.c --frontend--> AST --frontend--> LLVM IR --LLVM opt--> LLVM IR --LLVM llc--> .s Assembly --OS Assembler--> .o --OS Linker--> executable
```

### 命令作用

```shell
# 将源码转换为llvm IR
clang -emit-llvm -S multiply.c -o multiply.ll

# llvm IR -> llvm bitcode
llvm-as multiply.ll -o multiply.bc  # hexdump 查看二进制文件

# llvm bitcode -> llvm IR
lvm-dis multiply.bc -o multiply2.ll

# llvm IR -- opt --> llvm IR
opt –passname multiply.ll –o multiply.ll

# llvm bitcode -> binaray code
llc multiply.bc -o multiply.s # or  clang -S multiply.bc -o multiply.s –fomit-frame-pointer

# llvm link -> execute
llvm-link test1.bc test2.bc –o output.bc

# llvm execute
lli output.bc

# 语法树dump 
clang -cc1 test.c -ast-dump

```



在编译器架构中分为前端和后端，前端主要负责词法分析、语法分析生成llvm IR，后端负责

## 实现前端

#### 词法分析

词法分析器把程序代码的输入 流切分成token，而语法分析器则接受这些token并把token流构建成 AST（抽象语法树）。通常来说，被解析成token的语言是基于上下文无关语法的。一个token可以是一个字符串，由一个或多个同一范畴的字符组成。



## Kaleidoscope Language

只支持浮点64bit运算，类似于Python语法。并且允许使用外部函数通过`extern`

```python
# Compute the x'th fibonacci number.
def fib(x)
  if x < 3 then
    1
  else
    fib(x-1)+fib(x-2)

# This expression will compute the 40th number.
fib(40)

extern sin(arg);
extern cos(arg);
extern atan2(arg1 arg2);

atan2(sin(.4), cos(42))
```

当要开始实现以上语言时，就需要一个词法解析器。

