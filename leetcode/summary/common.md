# 通用技巧

## debug

变参打印debug
```cpp
#include <bits/stdc++.h>

using namespace std;

namespace DEBUG {
template <typename T>
inline void _debug(const char* format, T t) {
  cerr << format << '=' << t << endl;
}

template <class First, class... Rest>
inline void _debug(const char* format, First first, Rest... rest) {
  while (*format != ',') cerr << *format++;
  cerr << '=' << first << ",";
  _debug(format + 1, rest...);
}

template <typename T>
ostream& operator<<(ostream& os, const vector<T>& V) {
  os << "[ ";
  for (const auto& vv : V) os << vv << ", ";
  os << "]";
  return os;
}

#define debug(...) _debug(#__VA_ARGS__, __VA_ARGS__)
}  // namespace DEBUG

using namespace DEBUG;

int main(int argc, char* argv[]) {
  int a = 666;
  vector<int> b({1, 2, 3});
  string c = "hello world";

  // before
  cout << "a=" << a << ", b=" << b << ", c=" << c << endl;  // a=666, b=[ 1, 2, 3, ], c=hello world
  // 如果用printf的话，在只有基本数据类型的时候是比较方便的，然是如果要输出vector等的内容的话，就会比较麻烦

  // after
  debug(a, b, c);  // a=666, b=[ 1, 2, 3, ], c=hello world

  return 0;
}
```

## 容器

比较运算符重载

```cpp

struct student {
  string name;
  int score;
};

struct cmp {
  bool operator()(const student& a, const student& b) const {
    return a.score < b.score || (a.score == b.score && a.name > b.name);
  }
};

priority_queue<student, vector<student>, cmp> pq;

int main() {
  int n;
  cin >> n;
  for (int i = 1; i <= n; i++) {
    string name;
    int score;
    cin >> name >> score;
    pq.push({name, score});
  }
  student rk1 = pq.top();
  cout << rk1.name << ' ' << rk1.score << endl;
  return 0;
}
```


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

lowbit: 返回x的最后一位， 应用统计x的二进制中1的个数
```c++
x & -x

/*
6   110 & 010 = 010
*/
```

## 二分查找

```cpp
int lower_bound_a(vector<int>& nums, int target) {
    int i = 0, j = nums.size() - 1;
    while(i <= j) {
        int m = (i + j) / 2;
        if(nums[m] <= target) i = m + 1;
        else j = m - 1;
    }

    return j;
};


int upper_bound_a(vector<int>& nums, int target) {

    int i = 0, j = nums.size() - 1;
    while(i <= j) {
        int m = (i + j) / 2;
        if(nums[m] < target) i = m + 1;
        else j = m - 1;
    }

    return i;
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

基数排序、计数排序、插入排序、冒泡排序、归并排序是稳定排序。
选择排序、堆排序、快速排序、希尔排序不是稳定排序。

![sort](https://oi-wiki.org/basic/images/sort-intro-1.apng)

### 选择排序

非稳定排序O(n^2)
example `[7, 7, 3, 2, 4, 5]` 由于选择之后进行交换，所以7的位置错乱导致了其非稳定

```cpp
void selection_sort(int* a, int n) {
  for (int i = 0; i < n - 1; ++i) {
    int ith = i;
    for (int j = i + 1; j < n; ++j) {
      if (a[j] < a[ith]) {
        ith = j;
      }
    }
    std::swap(a[i], a[ith]);
  }
}
```

### 冒泡排序

稳定排序O(n^2), 大数不断沉底，小数不断上浮，一次循环至少排列出沉底的值。以下可以优化循环

```cpp
void bubble_sort(int *a, int n) {
  bool flag = true;
  while (flag) {
    flag = false;
    for (int i = 0; i < n - 1; ++i) {
      if (a[i] > a[i + 1]) {
        flag = true;
        int t = a[i];
        a[i] = a[i + 1];
        a[i + 1] = t;
      }
    }
  }
}
```

### 插入排序

从当前元素往前找合适的插入位置，前面的子序列一定是排序好的，算稳定排序O(n^2)。找到位置之后，将后面的元素往后移动一位。

```cpp
void insertion_sort(int* a, int n) {
  for (int i = 1; i <= n; ++i) {
    int key = a[i];
    int j = i - 1;
    while (j >= 0 && a[j] > key) {
      a[j + 1] = a[j];
      --j;
    }
    a[j + 1] = key;
  }
}
```
### 计数排序

计数排序的时间复杂度为O(n + w), 其中w代表待排序数据的值域大小。

```cpp
const int N = 100010;
const int W = 100010;

int n, w, a[N], cnt[W], b[N];

void counting_sort() {
  memset(cnt, 0, sizeof(cnt));
  for (int i = 0; i < n; ++i) ++cnt[a[i]];
  for (int i = 0; i < w; ++i) cnt[i] += cnt[i - 1];
  for (int i = n - 1; i >= 0; --i) b[cnt[a[i]]--] = a[i];
}
```

### 基数排序

...

### 快速排序

非稳定排序O(nlog n)
快速排序分为三个过程：

将数列划分为两部分（要求保证相对大小关系）；
递归到两个子序列中分别进行快速排序；
不用合并，因为此时数列已经完全有序。
```cpp
int partition(vector<int> &arr, int low, int high) {
    int pivot = arr[low];
    int i = low + 1, j = high;
    while (true) {
        while (i < high && arr[i] < pivot) i++;
        while (j > low && arr[j] > pivot) j--;
        if (i >= j) break;
        swap(arr[i], arr[j]);
        i++;
        j--;
    }
    swap(arr[low], arr[j]);
    return j;
}

void quicksort(vector<int> &arr, int low, int high) {
    if (low >= high) return;
    int position = partition(arr, low, high);
    quicksort(arr, low, position - 1);
    quicksort(arr, position + 1, high);
}
```

### 归并排序

归并排序（merge sort）是高效的基于比较的稳定排序算法。
归并排序基于分治思想将数组分段排序后合并，时间复杂度在最优、最坏与平均情况下均为O(nlog n)，空间复杂度为O(n)。

```cpp
void Merge(int arr[],int low,int mid,int high){
    //low为第1有序区的第1个元素，i指向第1个元素, mid为第1有序区的最后1个元素
    int i=low,j=mid+1,k=0;  //mid+1为第2有序区第1个元素，j指向第1个元素
    int *temp=new int[high-low+1]; //temp数组暂存合并的有序序列
    while(i<=mid&&j<=high){
        if(arr[i]<=arr[j]) //较小的先存入temp中
            temp[k++]=arr[i++];
        else
            temp[k++]=arr[j++];
    }
    while(i<=mid)//若比较完之后，第一个有序区仍有剩余，则直接复制到t数组中
        temp[k++]=arr[i++];
    while(j<=high)//同上
        temp[k++]=arr[j++];
    for(i=low,k=0;i<=high;i++,k++)//将排好序的存回arr中low到high这区间
  arr[i]=temp[k];
    delete []temp;//释放内存，由于指向的是数组，必须用delete []
}

void MergeSort (int arr [], int low,int high) {
  if(low>=high) { return; } // 终止递归的条件，子序列长度为1
  int mid =  low + (high - low)/2;  // 取得序列中间的元素
  MergeSort(arr,low,mid);  // 对左半边递归
  MergeSort(arr,mid+1,high);  // 对右半边递归
  Merge(arr,low,mid,high);  // 合并
}
```


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

vector<int> e[n];
for (auto &road : roads) {
    e[road[0]].push_back(road[1]);
    e[road[1]].push_back(road[0]);
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

