# CUTLASS

CUTLASS是CUDA c++实现高性能的矩阵乘法(GEMM)的模板抽象。



**cutlass::half_t**

cutlass::half_t 是cuda内建的数据类型struct，内部以uint16_t的方式存储数据storage。CUTLASS_HOST_DEVICE 这是一个host and device 都可以调用的函数，如果在device上调用，内部可以使用内部device函数。

```c++
struct alignas(2) half_t
{
    uint16_t storage;

    /// Default constructor
    CUTLASS_HOST_DEVICE half_t() : storage(0) {}
    /// Reinterpret cast from CUDA's half type
    CUTLASS_HOST_DEVICE explicit half_t(half const& x) : storage(reinterpret_cast<uint16_t const&>(x)) {}
    /// Floating point conversion
    CUTLASS_HOST_DEVICE explicit half_t(float x) { storage = convert(x).storage;}
    /// Integer conversion - round to nearest even
    CUTLASS_HOST_DEVICE explicit half_t(int x) { storage = convert(x).storage; }
    /// Integer conversion - round toward zero
    CUTLASS_HOST_DEVICE explicit half_t(unsigned x) { storage = convert(x).storage; }

    CUTLASS_HOST_DEVICE static half_t convert(float const& flt);
    CUTLASS_HOST_DEVICE static float convert(half_t const& x);
    /// FP32 -> FP16 conversion - rounds to nearest even
    CUTLASS_HOST_DEVICE static half_t convert(int const& n);
    /// FP32 -> FP16 conversion - rounds to nearest even
    CUTLASS_HOST_DEVICE static half_t convert(unsigned const& n);

    /// Assignment
    CUTLASS_HOST_DEVICE half_t& operator=(half const& x) { storage = reinterpret_cast<uint16_t const&>(x); return *this;}
    /// Converts to float
    CUTLASS_HOST_DEVICE operator float() const { return convert(*this); }
    /// Converts to float
    CUTLASS_HOST_DEVICE explicit operator int() const { return int(convert(*this)); }
    /// Casts to bool
    CUTLASS_HOST_DEVICE operator bool() const { return (convert(*this) != 0.0f); }
    /// Bitcasts to CUDA's half type
    CUTLASS_HOST_DEVICE half to_half() const {  return reinterpret_cast<half const&>(storage); }

    /// Accesses raw internal state
    CUTLASS_HOST_DEVICE uint16_t& raw() { return storage; }
    /// Accesses raw internal state
    CUTLASS_HOST_DEVICE uint16_t raw() const { return storage; }
};
```

cutlasss::half_t的核心函数在于convert， convert函数将任意的其他数据类型转换为half_t, 但是如果是对于half_t转换则是转换为float数据。

half_t的构造函数内部基本都是通过调用convert函数获取其内部的storage值，赋给自己的内storage。

然后是将half_t 转换为其他的数据类型的的运算符重载。

1.  other -> half_t(construct function by using convert function)
2. half_t -> other(operator overload)
3. get raw data 



这里需要分析以任意Tensor的RowMajor和ColMajor方式的描述。

一个矩阵为M, N大小，假设如图 3X4的矩阵

![image-20220426104510449](D:\Repo\learning_repo\cutlass\cutlass.assets\image-20220426104510449.png)

对于以上的数据在内存上通常存在两种排布，一种是在Row上内存连续的：3X4的矩阵的Row Major，是在同一行上的内存数据是连续的。3X4的矩阵在内存上还是3X4的的矩阵。

而如果这个3X4的矩阵如果是以ColMajor的排布，则内存上是在Col上是连续的。内存上可以看到的是4X3的排布。

![image-20220426104621867](D:\Repo\learning_repo\cutlass\cutlass.assets\image-20220426104621867.png)

所以对于任意矩阵，只有知道是RowMajor 还是ColMajor的数据之后才能知道其内存排布。



任意维度的Tensor，都存在一个维度，即坐标的大小cutlass::Coord, 用来描述Tensor的shape信息，

在cutlass::Coord 衍生出来cutlass::MatrixCoord, 用来描述矩阵的shape信息。



**cutlass::HostTensor**

