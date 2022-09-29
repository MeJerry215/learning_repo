# Effective C++/More Effective C++

1. 以const, enum, inline替换#define
2. 尽可能使用const
3. 确定对象在被使用前已先被初始化
4. 编译器默认为class创建default构造函数、copy构造函数、copy assignment操作符、以及析构函数
5. 若不想使用编译器自动生成的函数，显示的拒绝掉，比如设置为private
6. 为多态基类生命virtual析构函数
7. 析构函数需要处理异常
8. 绝不再构造和析构过程中调用virtual函数
9. 令operator=返回 reference to *this
10. 在operator中处理自我赋值
11. 赋值对象时需要考虑其每个成分
12. 以对象管理资源(RAII)
13. 在资源管理器类中小心copy行为
14. 在资源管理类中提供对原始资源的访问
15. new和delete采取相同形式
16. 以pass by reference to const替换pass by value
17. 返回对象的时，一般情况下谨慎返回reference
18. 尽量少做转型
19. 避免返回handles指向对象内部成分
20. 减少文件之间的编译依赖
21. public 继承是is-a关系
22. 绝不重新定义继承而来的non-virtual函数
23. 绝不重新定义继承而来的缺省参数，缺省参数是静态绑定的
24. 通过组合表达has-a关系
25. 审慎使用private继承
26. 审慎使用多重继承
27. typename的双重意义 typename C::const_iterator
28. 参数无关的代码抽离templates