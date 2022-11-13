# 通用技巧


## 数学

求解最小公约数，GCD辗转相除法
```c++
int GCD(int a, int b) {
    return b == 0 ? a : GCD(b, a % b);
}
```

求解最小公倍数，为 a * b = i * gcd  * j * gcd, gcd 两次乘 a * b / gcd = 最小公倍数
```c++
int LCM(int a, int b) {
    return a * b / GCD(a, b);
}
```


## 优先权队列应用

1. 大顶堆、小顶堆。
```cpp

//小顶堆
priority_queue <int,vector<int>,greater<int>> pri_que;
//大顶堆
priority_queue <int,vector<int>,less<int>> pri_que;
//默认大顶堆
priority_queue<int> pri_que;

```


## 字符串篇

1. 快速大小写转换
由于大小写之间差值为32，可以通过异或32 达到大小写转换。


## 数组篇

1. 对于给定数值范围[0, n-1]之间的数组，找出其中重复的数。

可以考虑原地交换，将对应的数放到对应位置上去。这种题目的通用解法可以考虑到hash，情况特殊可以考虑到异或的性质，甚至排序之后求解。

## 排序篇

使用辅助索引排序，排序索引，而不是实际元素，通常用在存在两个对应元素的容器，需要对其中一个排序，而另外一个容器也跟着排序。
通过排序后的辅助索引，顺序遍历即可得到排序的元素。

```cpp
vector<int> indexs(n);
iota(indexs.begin(), index.end(), 0);
sort(indexs.begin(), indexs.end(), [&](int i, int j) {
    return nums[i] < nums[j];
});
```

## auto推导

trick: 使用auto 推导快速在方法内定义函数
```cpp
auto next = [&](int cur) {
    return ((cur + nums[cur]) % n + n) % n; // 保证返回值在 [0,n) 中
};
```

## 图

连接边转换为邻接矩阵表示
```c++
for(auto edge: edges) {
    g[edge[0]].push_back(edge[1]);
    g[edge[1]].push_back(edge[0]);
}
```

遍历记录父节点和遍历深度
```c++
function<void(int, int, int)> dfs1 = [&](int sn, int fa, int d) {
    pa[sn] = fa;
    a[sn] = d;
    for(auto fn: g[sn]) if (fn != fa) dfs1(fn, sn, d + 1);
};
```

