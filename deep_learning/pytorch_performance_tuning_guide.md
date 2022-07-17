# Pytorch Performance Turning Guide

**如何计时gpu kernel**

gpu kernel异步执行，需要显式的插入同步操作



**data loading & augmentation**

Dataloader 使能`num_works` > 0 并行加载， `pin_memory=True`总是性能更好。

`Dali`能获得显著的数据加载提升。

![image-20220518190907581](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518190907581.png)



**computation optimization**

- 卷积网络使能cuDnn, `torch.backends.cudnn.benchmark=True`
- 增大batch size，最大利用gpu memory。 调整learning rate: 增加learning rate warmup
- 避免同步操作。比如print(cuda_tensor)、tensor.cuda(), cuda_tensor.cpu()，控制流`if(cuda_tensor!=0).all()`





...

![image-20220518191522937](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518191522937.png)

![image-20220518191539353](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518191539353.png)

![image-20220518191815879](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518191815879.png)

![image-20220518191857289](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518191857289.png)

![image-20220518192033739](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518192033739.png)

![image-20220518192216927](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518192216927.png)

![image-20220518192358199](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518192358199.png)

![image-20220518192557533](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518192557533.png)

![image-20220518192731880](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518192731880.png)

![image-20220518192801841](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518192801841.png)

![image-20220518193009401](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518193009401.png)

![image-20220518193307204](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518193307204.png)

![image-20220518193426328](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518193426328.png)

![image-20220518193557944](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518193557944.png)

![image-20220518193750063](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518193750063.png)

![image-20220518193921756](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518193921756.png)

![image-20220518195407242](D:\Repo\learning_repo\deep_learning\pytorch_performance_tuning_guide.assets\image-20220518195407242.png)













