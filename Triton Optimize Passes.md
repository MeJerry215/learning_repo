Triton 代码基于 https://github.com/triton-lang/triton/tree/release/3.0.x
commit 107465b86a80ef6d244c7b038bec418040e760a3

llvm 代码基于 10dc3a8e916d73291269e5e2b82dd22681489aa1


## make_ttir

### common.add_inliner(mlir::InlinerPass)
内置Pass 将函数内联，通常将jit函数调用其他jit函数给内联成一个函数。

### ttir.add_rewrite_tensor_pointer(mlir::triton::RewriteTensorPointerPass)
针对`triton::MakeTensorPtrOp`、`triton::AdvanceOp`、`triton::LoadOp`、`triton::StoreOp`、`scf::IfOp`、`scf::ForOp`、`scf::YieldOp`
#### triton::MakeTensorPtrOp
对应的是 op为`tt.make_tensor_ptr`，为将一个基地址根据stride和shape，转换为一个tensor类型
对这个op 核心改写则是将i32 的offset 转换为 i64 的offset

```c++
    // Cast I32 offsets into I64
    SmallVector<Value> i64Offsets;
    for (auto offset : op.getOffsets()) {
      auto i64Offset = builder.create<arith::ExtSIOp>(op.getLoc(), builder.getI64Type(), offset);
      i64Offsets.push_back(i64Offset);
    }
```

#### triton::AdvanceOp
对应的是 op为`tt.advance`，对一个指针取偏移。
此重写的的核心操作是将i32 offset 转换为 i64 offset 同时 加上偏移
```c++
    for (int i = 0; i < info.length(); ++i) {
      Value i64Offset = builder.create<arith::ExtSIOp>(
          op.getLoc(), builder.getI64Type(), op.getOffsets()[i]);
      Value newOffset = builder.create<arith::AddIOp>(
          op.getLoc(), info.getOffset(i), i64Offset);
      newOffsets.push_back(newOffset);
    }
```

#### triton::LoadOp 和 triton::StoreOp
对应的是`tl.load`和`tl.store`操作
核心是替换掉原来的load store以同样的op进行替换，不过新op的入参会改变：newPtr、newMask、newOther。
newPtr 、newMask、newOther 都是使用了I64 作为偏移的计算，其他没有改变。
```c++
    auto newPtr = info.generatePtr(builder, op->getLoc());
    auto newMask = info.generateMask(builder, op->getLoc(), boundaryCheck);
    Value newOther;
    if (auto loadOp = dyn_cast<triton::LoadOp>(op))
      newOther = info.generateOther(builder, op->getLoc(), loadOp.getPadding());
```
#### scf::IfOp
If 语句中如何循环产生了yield 新值，这个时候这个新值的指针就会被替换为I64类型。

#### scf::ForOp


#### scf::YieldOp

## make_ttgir


## make_llir



## make_ptx




## make_cubin

