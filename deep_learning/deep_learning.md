# Deep Learning

机器学习分为监督学习和无监督学习。大部分学习算法都可以使用**随机梯度下降法**求解

**样本**是指从系统处理的对象或事件中收到**量化的特征集合**，从样本中学的模型的过程称之为**训练**。

机器学习的目标是使学得的模型更好的适用于**新样本**，模型适用于新样本的能力称为**泛化**

**错误率**：分类错误的样本占样本总数的比例。 $E = a / m$, $a$表示分类错误，$m$表示样本数目，准确率则是 $1 -E$

在训练集上的误差称之为训练误差，而在新样本上的称为泛化误差。

常见的机器学习任务：

- 分类：指定输入属于k类中的哪一类。$y = f(x)$ 。x为输入图片，y为输出的分类的概率分布。
- 输入缺失分类
- 回归：对于给定输入预测数值，在预测索赔金额或者证券未来的价格。
- 转录
- 机器翻译
- 结构化输出
- 异常检测
- 合成和采样
- 缺失值填补
- 去噪
- 密度估计或者概率质量函数估计

**性能度量P**：评估机器学习算法的能力。对于分类采用`准确率` or `错误率`。对于不同的任务使用不同的性能度量。

**测试集**: 评估系统性能，和训练系统的训练数据分开。

**无监督学习**：训练含有多特征数据集，然后学习数据集的结构性质。

**监督学习**：训练很多特征数据集，每个样本都存在`标签(label)`或`目标(target)`

**容量和过拟合、欠拟合**

在之前未观测到的输入上表现良好的能力称为**泛化**。

在某个训练集上，训练计算结果的度量误差称为**训练误差**

通常会假设训练集和测试集**独立同分布**的。

所以机器学习的效果好坏受一下因素决定：

1. 降低训练误差(欠拟合)
2. 缩小训练误差和测试误差的差距(过拟合)

**容量(Capacity)**低的模型可能很难拟合训练数据集产生**欠拟合**；容量高的可能会产生**过拟合**，不适用于测试集。

 **权重衰减**：通过$ J{(w)} = MSE_{train} + \lambda w^Tw$，来控制倾向于更小的权重，当$\lambda$越大，权重倾向越小。这种代驾函数也被称为正则化项。需要注意的是这只是一种正则化的方式，可以以其他的函数衡量正则化权重衰减惩罚。

**超参数**：控制算法行为，超参数本身不是通过学习算法学习出来的。

**k-折交叉验证算法**：将数据分为k个不重合的子集，当第i次测试时，数据第i次子集用于测试集。其他数据集用于训练集。

对于二分类问题，可以划分为真正例(True Positive)、 假正例(False Positive)、真反例(True Negative)、假反例(False Negative)。即TP、FP、TN、FN。

分为查准率 P 和查全率 R，当查准率高，查全率会偏低，即准确度高的时候，标准也随之提高，会造成假反例的提高。简单任务可以都很高。
$$
 P = \frac {TP}{TP + FP} \\
 R = \frac {TP}{TP + FN}
$$


## 线性回归

线性回归定义为$\widehat {y} = w^Tx$。$w^T$ 是作为参数，假设有m个样本的输入则记作矩阵$X^{(test)}$，回归目标$y^{(test)}$。度量计算模型误差使用均方误差(MSE)。
$$
MSE_{test} = \frac{1}{m} \sum_{i}{}{({\widehat{y}}^{(test)} - y^{(test)})^2}
$$


当算法完美拟合时，即MSE的误差最小。这个时候需要反复更新权重$w$。MSE关于$w$，对其求微分方程。
$$
\nabla_w MSE_{train} = 0 \\
\Rightarrow \frac{1}{m}\nabla_w {\lVert X^{(train)}w - y ^ {(train)}\rVert}^2 = 0 \\
\Rightarrow \frac{2}{m}\nabla_w ({X^{(train)T} X^{(train)}}w - {X^{(train)T}}y^{(train)}) = 0 \\
\Rightarrow w = ({X^{(train)T} X^{(train)}})^{-1}{X^{(train)T}}y^{(train)}
$$


通常来说，会带上一个额外的参数$b$， 使得$\widehat {y} = w^Tx + b$

![image-20220525165728600](D:\Repo\learning_repo\deep_learning\deep_learning.assets\image-20220525165728600.png)

```python
import numpy as np
'''
X (100, 6)
W (6)
B (1)
Y (100)
'''
batch_size = 100

def forward(x, w, b):
    return np.dot(x, w) + b

def backward(x, w, b, y, y_exp, lr=1e-3):
    b_gradient = (y - y_exp)
    w_gradient = (y - y_exp) * x
    b = b - b_gradient * lr
    w = w - w_gradient * lr
    return w.reshape(6,), b

np.random.seed(99)
x = np.random.random((batch_size, 6)) - 0.5
w = np.zeros((6,))
b = np.zeros((1,))

w_expect = np.array([0.3, 0.2, 0.5, -0.1, 1.3, -2.3])
b_expect = np.array([-3.5])
y_expect = forward(x, w_expect, b_expect)
loss = 1.0
i = 0
while loss > 1e-15:
    batch = x[i % 100].reshape(1, -1)
    y = forward(batch, w, b)
    l1_loss = y - y_expect[i % 100]
    w, b = backward(batch, w, b, y, y_expect[i % 100])
    loss = 2 * l1_loss * l1_loss
    i = i+1

print(loss, w, b, i)
np.testing.assert_allclose(y_expect, forward(x, w, b), 1e-3, 1e-3)
```



