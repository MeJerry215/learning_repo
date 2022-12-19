# C-HOI

[Cascaded Human-Object Interaction Recognition Github Pytorch Implements](https://github.com/tfzhou/C-HOI)

[Cascaded Human-Object Interaction Recognition Paper](https://openaccess.thecvf.com/content_CVPR_2020/papers/Zhou_Cascaded_Human-Object_Interaction_Recognition_CVPR_2020_paper.pdf)

HOI(Human Object Interaction), 即是人-物体交互检测，主要目的是定位人体、物体、并识别他们之间的交互关系，就是检测图像中的<人体，动词，物体>三元组。HOI检测旨在利用人体、物体以及人物对的特征将人与物体之间的交互进行关联，从而实现对图像或视频中的动作分类。

典型的例子如下，<human, eat, carrot>, 所以HOI需要能够准确的识别出(1) 正在交互的主客体，以及(2) 预测出行为。

![image-20220818171001354](D:\Repo\learning_repo\models\C-HOI.assets\image-20220818171001354.png)

CHOI是级联多stage网络，由目标检测和交互识别网络组成。RRM(Relation Ranking Module)根据概率排序可能的人类识别， RCM(Relation Classification Module)从之前提取出来的高置信度的结果中识别出动词

RRM instance location network输出Human和Objection的区域，然后human-object pair送进到识别网络RCM中，
