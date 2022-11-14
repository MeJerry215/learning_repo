# 通用技巧

## 哈希

在标准库实现里，每个元素的散列值是将值对一个质数取模得到的，更具体地说，是 这个列表 中的质数(g++ 6 及以前版本的编译器，这个质数一般是 ，g++ 7 及之后版本的编译器，这个质数一般是)。自定义hash, 防hack。

```cpp
struct my_hash {
  static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }

  size_t operator()(uint64_t x) const {
    static const uint64_t FIXED_RANDOM =
        chrono::steady_clock::now().time_since_epoch().count();
    return splitmix64(x + FIXED_RANDOM);
  }

  // 针对 std::pair<int, int> 作为主键类型的哈希函数
  size_t operator()(pair<uint64_t, uint64_t> x) const {
    static const uint64_t FIXED_RANDOM =
        chrono::steady_clock::now().time_since_epoch().count();
    return splitmix64(x.first + FIXED_RANDOM) ^
           (splitmix64(x.second + FIXED_RANDOM) >> 1);
  }
};

unordered_map<int, int, my_hash> my_map;
```

## 并查集

并查集的通用写法为:

```c++
struct dsu {
  vector<int> pa;

  explicit dsu(int size) : pa(size) { iota(pa.begin(), pa.end(), 0); }

  int find(int x) { return pa[x] == x ? x : pa[x] = find(pa[x]); }

  void merge(int x, int y) { pa[find(x)] = find(y); }
};
```


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

判断回文串, 目前存在的方法是中心扩展、动态规划。而中心扩展存在一个问题就是回文中心位置的是否统一
以n = 4为例, 产生的回文串的左右边界有

| 编号 |  左起点 | 右起点 |
|-----| --------| -------|
|  0  |   0     | 0      |
|  1  |   0     | 1     |
| 2   |  1      |  1    |
| 3   |  1      | 2     |
| 4   | 2       | 2     |
| 5   | 2       | 3     |
| 6   | 3       | 3     |

存在 k = 2 * n - 1 个中心，所以遍历中心进行扩展 l = (k / 2) r = l + l mod 2
```cpp
for (int i = 0; i < 2 * n - 1; ++i) {
    int l = i / 2, r = i / 2 + i % 2;
    while (l >= 0 && r < n && s[l] == s[r]) {
        --l;
        ++r;
        ++ans;
    }
}
```


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

计算排序需要的置换次数，数值离散
```cpp
vector<int> B = A;
/* 预期排列 */
sort(B.begin(), B.end());
/* 预期排列的index */
unordered_map<int, int> mp;
for (int i = 0; i < B.size(); i++) mp[B[i]] = i;

int ret = 0;
for (int i = 0; i < A.size(); i++) {
    // 不断将 A[i] 交换到目标位置，直到 A[i] = B[i]
     while (A[i] != B[i]) {
        swap(A[i], A[mp[A[i]]]);
        ret++;
    }
}
return ret;
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

