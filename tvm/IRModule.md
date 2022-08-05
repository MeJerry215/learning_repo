# IRModule

IRModule作为tvm中的核心数据结构，主要提供承载函数的功能。图优化、lower的操作都是对IRModule。IRModule也秉持着tvm的设计思想对象和数据分离，通过持有IRModuleNode实现数据的封装。而IRModule提供了通用的接口。提供从functions、expr、文本txt构造IRModule。

```c++
class IRModule : public ObjectRef
{
public:
    TVM_DLL explicit IRModule(Map<GlobalVar, BaseFunc> functions,
                            Map<GlobalTypeVar, TypeData> type_definitions = {},
                            std::unordered_set<String> import_set = {}, parser::SourceMap map = {},
                            DictAttrs attrs = {});
    
    IRModuleNode *operator->() const
    {
        auto *ptr = get_mutable();
        ICHECK(ptr != nullptr);
        return static_cast<IRModuleNode *>(ptr);
    }

    static std::pair<IRModule, GlobalVar> FromExprInContext(
        const RelayExpr &expr, const Map<GlobalVar, BaseFunc> &global_funcs = {},
        const Map<GlobalTypeVar, TypeData> &type_definitions = {},
        std::unordered_set<String> import_set = {});

    TVM_DLL static IRModule FromExpr(const RelayExpr &expr,
                                     const Map<GlobalVar, BaseFunc> &global_funcs = {},
                                     const Map<GlobalTypeVar, TypeData> &type_definitions = {});

    TVM_DLL static IRModule FromText(const String &text, const String &source_path);

    using ContainerType = IRModuleNode;

    static constexpr bool _type_is_nullable = false;

    TVM_DEFINE_OBJECT_REF_COW_METHOD(IRModuleNode);
};


class IRModuleNode : public Object {
private:
    /*! \brief A map from string names to global variables that ensures global uniqueness. */
    Map<String, GlobalVar> global_var_map_;

    /*! \brief A map from string names to global type variables (ADT names) that ensures global uniqueness. */
    Map<String, GlobalTypeVar> global_type_var_map_;

    /*! \brief A map from constructor tags to constructor objects for convenient access */
    std::unordered_map<int32_t, Constructor> constructor_tag_map_;

public:
    /*! \brief A map from ids to all global functions. */
    Map<GlobalVar, BaseFunc> functions;
    /*! \brief A map from global type vars to ADT type data. */
    Map<GlobalTypeVar, TypeData> type_definitions;
    /*! \brief The source map for the module. */
    parser::SourceMap source_map;
    /* \brief Additional attributes storing meta-data about the module. */
    DictAttrs attrs;

}
```

`IRModuleNode` 内部的变量包含一个global_var_map，即所有的全局变量名的map。`global_type_var_map_` 全局变量类型map。`constructor_tag_map_`构造函数map。以上三个为内部私有数据。

- functions：持有的函数map，尤其是这个是重点，global var可以理解为函数名，basefunc则是函数声名
- type_definitions：类型定义map
- source_map：源码map，其次这个是codegen 生成的源代码，函数的实现
- attrs：IRModule关联的attrs map

```c++
// function 操作函数，增、删、改、查
TVM_DLL void Add(const GlobalVar &var, const BaseFunc &func, bool update = false);
TVM_DLL void AddUnchecked(const GlobalVar &var, const BaseFunc &func);
TVM_DLL void Update(const GlobalVar &var, const BaseFunc &func);
TVM_DLL void Remove(const GlobalVar &var);
TVM_DLL bool ContainGlobalVar(const String &name) const;
TVM_DLL GlobalVar GetGlobalVar(const String &str) const;
TVM_DLL Array<GlobalVar> GetGlobalVars() const;
TVM_DLL BaseFunc Lookup(const GlobalVar &var) const;
TVM_DLL BaseFunc Lookup(const String &name) const;
// typedef 操作函数、增、删、改、查
TVM_DLL void AddTypeDef(const GlobalTypeVar &var, const TypeData &type, bool update = false);
TVM_DLL void AddTypeDefUnchecked(const GlobalTypeVar &var, const TypeData &type, bool update = false);
TVM_DLL void UpdateTypeDef(const GlobalTypeVar &var, const TypeData &type);
TVM_DLL bool ContainGlobalTypeVar(const String &name) const;
TVM_DLL GlobalTypeVar GetGlobalTypeVar(const String &str) const;
TVM_DLL Array<GlobalTypeVar> GetGlobalTypeVars() const;
TVM_DLL TypeData LookupTypeDef(const GlobalTypeVar &var) const;
TVM_DLL TypeData LookupTypeDef(const String &var) const;
// IRModule函数
TVM_DLL void Update(const IRModule &other);
// 拷贝
TVM_DLL IRModule ShallowCopy();
```



## BuildRelay

`BuildRelay`对应的是tvm中的`relay.build`接口，是将从前端导入生成的mod，经过图优化，然后lower 生成代码，最后生成runtime的整个端到端流程。



