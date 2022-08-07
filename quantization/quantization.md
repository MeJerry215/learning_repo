# Quantization

## Basic

[模型量化（int8）知识梳理](https://zhuanlan.zhihu.com/p/516116539)

量化分为：线性量化和非线性量化，线性量化时，信号的幅度划分是等间隔的划分，非线性量化时非等间隔的(比如pow2、log量化)。

常用的量化方式主要是线性量化：对称量化和非对称量化

量化粒度：per-tensor，per-channel，量化粒度越小，精度越好，成本越高

量化：float32 -> int8 的过程，计算公式如下，当b为0的时候就是对称量化，看是对称量化还是非对称量化主要就是看是否存在零点。
$$
y = w * x_q + b_f
$$


反量化：int8 -> float32的过程

对于量化：对称量化可以满足绝大部分的需求，weight量化非对称量化比对称量化代价大，激活函数的量化一般使用非对称量化，或者成uint8。

![非对称量化和对称量化](https://img-blog.csdnimg.cn/img_convert/3e1797c9e43ac8f9bc223718a589d505.png)

在非对称量化场景下，$\alpha$和$\beta$是不相等的，如果要从float量化到int8则需要计算$w$和$b$
$$
w = \frac{(\alpha - \beta)}{x_{max} - x_{min}} = 7 / 255 = 0.0274509804\\ 
b_f = y - w * x = 4 - 0.0274509804 * 127 = 0.5137254892
$$

代入进行验证一下，如果是在-128, 则$y = w * x + b$ 计算的结果为 -3.000000002, 如果以int表示b, 则公式变为

$$
y = (x_q + b_q) * w \\
b_q * w = b_f \\
b_q = round(\frac{b_f} {w})
$$

算出来的b_q = 0.5137254892 / 0.0274509804 = 19

对称量化场景下，$\alpha$和$\beta$是相等的，则无须计算b, 此时只需要计算
$$
    w = \frac{\alpha}{x_{max}} = 0.0236220472 
$$
相对来说说简化了网络的部署，精度稍微降低了一点。在TensorRT中使用的就是对称量化。

在网络中选择合适的$\alpha$和$\beta$是相对来说比较核心的问题，以下就简化过程只对对称量化进行论述。

在网络中量化可能存在某部分极值，而这部分极值所占比例极低，而如果将这些极值作为我们选择的$\alpha$参数来计算$w$，那么每一阶能够表示的浮点计算结果偏差就大。

量化本身就是以牺牲一定的精度表示，带来性能的提升。举个极端的例子$\alpha$选择4 和10的情况下，算出来的$w$分别为0.0314960630和0.0787401575在对称量化场景下。而如果要表示0.5 这个数则计算为16和6，如果进行反量化之后得到的真实浮点值为0.503937008 和 0.472440945。则显而易见，选择$\alpha$ 为4的时候对于表示0.5的量化较为好。

所以就存在丢弃掉一部分的极值，将这部分超过$\alpha$的饱和截取为$\alpha$。所以如何选择合适的$\alpha$ 就存在多种算法: `percentile`、`KL-divergence`。而这个过程也就是被称为校准的过程，通过校准获得使得计算结果相对来说精度较高的$\alpha$


这里推导一下以对称量化方式做add 和矩阵乘法的量化。
### Add


$$
    c = a + b \\
    c = (w_a * x_a) + (w_b * x_b) \\
    c = w_a * (x_a + \frac{w_b}{w_a} * x_b)
$$

如果下一层c也进行了量化，预期输出的结果是Int8.则公式转变为

$$
    w_c * x_c = w_a * (x_a + \frac{w_b}{w_a} * x_b) \\
    x_c = \frac{w_a}{w_c} * (x_a + \frac{w_b}{w_a} * x_b)
$$

### Dense

$$
    c = \sum_{i=1}^{n} a * b \\
    c = \sum_{i=1}^{n} (w_a * w_b * x_a * x_b) \\
    c = (w_a * w_b) * \sum_{i=1}^{n}(x_a * x_b)
$$

如果下一层c也进行了量化

$$
    w_c * x_c = (w_a * w_b) * \sum_{i=1}^{n}(x_a * x_b) \\
    x_c = \frac{(w_a * w_b)}{w_c}  * \sum_{i=1}^{n}(x_a * x_b)
$$


## 使用量化来计算，代码实战

```python
import numpy as np

def gen_random(shape, low=0, high=1, dtype="float32"):
    if dtype == "float32":
        return np.random.uniform(low=low, high=high, size=shape)
    elif dtype == "int8" or dtype == "int32":
        return np.random.randint(low=low, high=high, size=shape)
    return None


def quant_tensor(x, w, b, a_min=-128, a_max=127):
    return np.clip(np.round((x - b) / w), a_min=a_min, a_max=a_max).astype("int8")


def dequant_tensor(x, w, b):
    return w * x + b


def calculate_quant_param(x, with_bias=True):
    a_min = np.min(x) 
    a_max = np.max(x)
    if not with_bias:
        a_max = max(abs(a_min), abs(a_max))
        a_min = -a_max
    if with_bias:
        w = (a_max - a_min) / 255
        b = a_max - w * 127
    else:
        w = a_max / 127
        b = 0
    print("a_max {} a_min {}".format(a_max, a_min))
    return w, b

shape = (10000)

np.random.seed(42)
x1 = gen_random(shape=shape, low=-1, high=1, dtype="float32")
x2 = gen_random(shape=shape, low=-2, high=1, dtype="float32")

w1, b1 = calculate_quant_param(x1)
w2, b2 = calculate_quant_param(x2)
print(w1, b1, w2, b2)
    
exp = x1 + x2

q1 = quant_tensor(x1, w1, b1)
q2 = quant_tensor(x2, w2, b2)
# print(q1, q2)

f1 = dequant_tensor(q1, w1, b1)
f2 = dequant_tensor(q2, w2, b2)
c = f1 + f2
# print(c)
# print(exp)

np.testing.assert_allclose(c, exp, atol=0.01, rtol=0.01)
```

这个时候量化时能够算出正确答案的，但是非常不幸的是，我们在x1中混入一个叛徒， 这个叛徒的值为10。由于a_max 变为了10，所以这个值导致我们算出来的w1会相对来说偏大, 在原来的场景中计算出来的值为0.007840831674751093， 而在加入叛徒之后，我们算出来的的结果为
0.04313716364897752。

所以每变化1的int8数值，实际产生的效果为0.04313716364897752的数值变，必然大于atol 0.01。

所以我们就得有办法去去除掉这种不合理的极值。

这里就使用不带bias来模拟percentile方式的量化, 在percentile范围内的数都认为是有效数字，其余的极值丢弃。所以在这种场景下得到将会是除了极值10以为的所有数值，然后寻找min，max进行量化。
增加一个函数来获取对应
```python
def _find_scale_by_percentile(arr, percentile=0.99999):
    x = np.abs(arr)
    max_k = int(x.size * percentile)
    return np.partition(x, max_k)[0: max_k]
```

如果使用原先没有percentile丢弃有效位的方式进行量化，计算出来的$w_1$ 为 0.07874015748031496, 在进行丢弃之后计算之后的$w_1$为0.00787383252353754。

使用这种方式进行计算得到并比较结果

```shell
# origin not using percentile
Mismatched elements: 5103 / 10000 (51%)
Max absolute difference: 0.04658287
Max relative difference: 38.4349307
 x: array([ 9.118319, -0.141494, -0.991779, ..., -0.220215, -0.913226,
       -2.031146])
 y: array([ 9.120922, -0.099835, -1.00755 , ..., -0.195489, -0.875064,
       -2.048925])


# using percentile
Mismatched elements: 3 / 10000 (0.03%)
Max absolute difference: 9.00250153
Max relative difference: 3.37418477
 x: array([ 0.118421, -0.109875, -0.999456, ..., -0.19646 , -0.881629,
       -2.04667 ])
 y: array([ 9.120922, -0.099835, -1.00755 , ..., -0.195489, -0.875064,
       -2.048925])
```

可以看到虽然存在误差，但是相对来说整体的误差更小了。

`percentile`是相对来说比较朴素的一种选择a_min和a_max
的一种方式。对于我们这一层的量化后的误差其实并没有做很好的评估，而kl_divergence方法则是一种应用数值分析思想，是用来描述两个概率分布P和Q的差异的一种方法。多应用于概率论和信息论中。在信息论中D(P||Q)表示用概率分布Q来拟合真实分布P时，产生的信息损耗，其中P表示真实分布，Q表示P的拟合分布。

![kl_divergence](https://img-blog.csdnimg.cn/img_convert/bdd4983c4839cdb21c3fdfa98fee9a98.png#pic_center)

$$
    D(P||Q) =  \sum_{i}^{} P(i) * log(\frac{P(i)}{Q(i)})
$$

根据以上公式，我们可以推算出如果P(i)非常相似则$\frac{P(i)}{Q(i)} = 1$, 则最终计算结果为0 

## KL_divergence

https://blog.51cto.com/u_15357586/3927343

