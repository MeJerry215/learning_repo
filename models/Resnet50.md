# Resnet50

论文 [Deep Residual Learning for Image Recognition](https://openaccess.thecvf.com/content_cvpr_2016/papers/He_Deep_Residual_Learning_CVPR_2016_paper.pdf)

背景：深层神经网络在分类任务上有一系列的突破，从一系列的实践可以看出模型的深度越深模型的精度也越高。通过堆叠网络层数获得更好的精度，也开始出现一些问题。随着深度的加深就会产生**梯度消失/爆炸**，从而使得网络难以收敛。虽然现在可以通过归一化可以**缓解**该问题，但是更深层网络也开始变得达到预期的效果。退化问题就开始暴露出来，训练过一段时间精度达到饱和，然后快速的衰减，这并不是由于过拟合导致的。

![image-20220818144159958](D:\Repo\learning_repo\models\Resnet50.assets\image-20220818144159958.png)

这就很违背认知，即模型通过更少的层得到一个相对较高的精度之后，在这之后添加层能够更深层次的特征，理论上应该精度更高，但是实际表现出来，精度反而下降了。以前通过堆叠网络层数的方式可以看做是`identity mapping`， 即$H(x)$， 现在将这种方式变为`residual mapping`，即$F(x) = H(x) - x$，最终网络层变为$H(x) = F(x) + x$。即网采用下图中结构`shortcuts`

![image-20220818145715357](D:\Repo\learning_repo\models\Resnet50.assets\image-20220818145715357.png)

网络结构：

1. input/output feature map size 相同，则具有相同的channel

2. input/output feature map 减半, 则channel x 2 

3. 针对1中，shortcuts直接使用

4. 针对2中, 使用1x1卷积升维，同时使用stride2 减半feature map size

以Resnet-34为例子， 实线就是3场景, 而4是虚线场景。

![image-20220818160730083](D:\Repo\learning_repo\models\Resnet50.assets\image-20220818160730083.png)

以下是各种网络的参数配置

![image-20220818160920735](D:\Repo\learning_repo\models\Resnet50.assets\image-20220818160920735.png)

![image-20220818160941913](D:\Repo\learning_repo\models\Resnet50.assets\image-20220818160941913.png)

可以看到使用residual block之后，resnet-18同样获得和plain-18相似的性能，同时也解决了更深网络精度下降的，即resnet-34比resnet-18具有更高的网络精度。最后贴上张网络精度图

![image-20220818161248504](D:\Repo\learning_repo\models\Resnet50.assets\image-20220818161248504.png)