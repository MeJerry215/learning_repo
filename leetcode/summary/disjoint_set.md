# 并查集专题

并查集是一种用于管理元素所属集合的数据结构，实现为一个森林，其中每棵树表示一个集合，树中的节点表示对应集合中的元素。

顾名思义，并查集支持两种操作：

- 合并（Union）：合并两个元素所属集合（合并对应的树）
- 查询（Find）：查询某个元素所属集合（查询对应的树的根节点），这可以用于判断两个元素是否属于同一集合

并查集在经过修改后可以支持单个元素的删除、移动；使用动态开点线段树还可以实现可持久化并查集。


## 初始化

初始时，每个元素都位于一个单独的集合，表示为一棵只有根节点的树。方便起见，我们将根节点的父亲设为自己。

```cpp
struct dsu {
  vector<int> pa;

  explicit dsu(int size) : pa(size) { iota(pa.begin(), pa.end(), 0); }
};
```

## 查询

我们需要沿着树向上移动，直至找到根节点。

```cpp
int dsu::find(int x) { return pa[x] == x ? x : find(pa[x]); }
```

![img1](https://oi-wiki.org/ds/images/disjoint-set-find.svg)

由于在union的过程中最差情况下，简历的树的深度就是n，数退化为链表形式，所以使用路径压缩，将相同节点的子节点都挂接到父节点下。
查询过程中经过的每个元素都属于该集合，我们可以将其直接连到根节点以加快后续查询。

![img2](https://oi-wiki.org/ds/images/disjoint-set-compress.svg)

```cpp
int dsu::find(int x) { return pa[x] == x ? x : pa[x] = find(pa[x]); }
```

## 合并

要合并两棵树，我们只需要将一棵树的根节点连到另一棵树的根节点。

![img3](https://oi-wiki.org/ds/images/disjoint-set-merge.svg)

```cpp
void dsu::merge(int x, int y) { pa[find(x)] = find(y); }
```

总体代码如下

```cpp
struct dsu {
  vector<int> pa;

  explicit dsu(int size) : pa(size) { iota(pa.begin(), pa.end(), 0); }

  int find(int x) { return pa[x] == x ? x : pa[x] = find(pa[x]); }

  void merge(int x, int y) { pa[find(x)] = find(y); }
};
```