# CMD





```shell
# 解决vscode 连接docker
sudo groupadd docker          #添加docker用户组
sudo gpasswd -a $USER docker  #将当前用户添加至docker用户组
newgrp docker                 #更新docker用户组

# 查询apt 软件安装位置
dpkg -L 软件名

# ping 工具安装
apt-get install inetutils-ping

# git 忽略权限配置
git config --global core.filemode false

# git 配置默认编辑器vim
git config --global core.editor "vim"

# 查看所有分支
git branch -a

# checkout 分支
git checkout -b xxxx（本地分支名称） yyyy(上条命令查找到的远程分支的名称)

# 同步所有远程分支
git branch -r | grep -v '\->' | while read remote; do git branch --track "${remote#origin/}" "$remote"; done

git pull --all

# 大家在提交代码时，在机器上把git config global设置都给清除了。
git config --global user.name "lei.zhang1"
git config --global user.email "lei.zhang1@iluvatar.ai"

# 统一在自己项目目录使用--local参数：
git config --local user.name "lei.zhang1"
git config --local user.email "lei.zhang1@iluvatar.ai"


# nfs 挂载
mount \\192.168.1.4\home\pi\Server x:
mount \\10.201.40.28\home\lei.zhang x:


# docker
docker run -it --name igie_zhanglei_0513 --privileged --cap-add=ALL -v /dev:/dev -v /root:/root -v /lib/modules:/lib/modules -v /usr/src:/usr/src -v /home/lei.zhang:/home/lei.zhang 10.150.9.98:80/performance/bi_img:20220513_igie /bin/bash

docker run --name "container_bi_20220422" -it --privileged --pid=host --cap-add=ALL -v /dev:/dev -v /lib/modules:/lib/modules -v /usr/src:/usr/src -v /home/lei.zhang:/home/lei.zhang container_bi_20220422-1130 /bin/bash

docker run --name "tvm0.9dev_leizhang" -it --privileged --pid=host --cap-add=ALL -v /dev:/dev -v /lib/modules:/lib/modules -v /usr/src:/usr/src -v /home/lei.zhang:/home/lei.zhang 2912baf2ad1 /bin/bash

docker run --name "bi_img_leizhang_20220210" -it --privileged --pid=host --cap-add=ALL -v /dev:/dev -v /lib/modules:/lib/modules -v /usr/src:/usr/src -v /home/lei.zhang:/home/lei.zhang -v /mnt/nas/home/lei.zhang/:/mnt/nas/home/lei.zhang/ 10.150.9.98:80/performance/bi_img:20220210 /bin/bash

docker run --name "bi_img_leizhang_c" -it --privileged --pid=host --cap-add=ALL -v /dev:/dev -v /lib/modules:/lib/modules -v /usr/src:/usr/src -v /home/lei.zhang:/home/lei.zhang  container_bi_20220422-1130:1.0 /bin/bash

sudo docker run --name "tvm0.9dev_leizhang_21.10" --shm-size=16g --ulimit --ulimit stack=67108864 -it -v /home/lei.zhang/:/home/lei.zhang/ --privileged  nvcr.io/nvidia/tensorrt:21.10-py3

## docker 拉起pytorch gpu镜像
sudo docker run  --name "zhanglei.tensorrt_22.02" --ulimit memlock=-1 --ulimit stack=67108864 -it  -v /home/lei.zhang/:/home/lei.zhang/ --privileged  nvcr.io/nvidia/tensorrt:22.02-py3
sudo nvidia-docker run --gpus all --shm-size=16g --ulimit memlock=-1 --ulimit stack=67108864 -it -v /home/lei.zhang/:/home/lei.zhang/ --privileged  nvcr.io/nvidia/tensorrt:22.02-py3

docker run --name "mr_test_0816_zhanglei" -it --privileged --pid=host --cap-add=ALL -v /dev:/dev -v /lib/modules:/lib/modules -v /usr/src:/usr/src -v /home/lei.zhang:/home/lei.zhang 10.150.9.98:80/sw_test/apps:2.3.0.20220816.86-centos-7.8.2003-x86_64-10.2-python3.6_mr /bin/bash

sudo docker exec -it zhanglei.tensorrt_22.02 /bin/bash
bi_img_leizhang_20220217
sudo docker exec -it bi_img_leizhang_20220217 /bin/bash
sudo docker exec -it container_leizhang_20220606-1756 /bin/bash
container_bi_20220422-1130
sudo docker exec -it container_bi_20220422-1130 /bin/bash
sudo docker exec -it zhanglei_tensorrt_22.02 /bin/bash
sudo docker exec -it container_bi_20220422 /bin/bash
reverent_lamarr
# 将运行的镜像保存为可加载镜像，以便后续拉起
sudo docker commit 14478deb892d tvm_gpu_test_images
sudo docker run --name "gpu_tvm_v1_zhanglei" --gpus all --shm-size=1g --ulimit memlock=-1 --ulimit stack=67108864 -it --rm -v /home/lei.zhang/:/home/lei.zhang/ -v /mnt/nas/:/mnt/nas/    2912baf2ad18

sudo -s
# adduser user.name --force-badname

useradd  -m -d /home/fanwu.han -s /bin/bash fanwu.han
usermod -a -G sudo fanwu.han

# 格式化时间
date "+%Y_%m_%d %H:%M:%S"  # 2022_03_11 10:00:13
date "+%Y_%m_%d_%H_%M" # 2022_03_11_10_01

apt update && apt install -y sshfs
sshfs -o allow_root,default_permissions root@10.113.1.9:/mnt/nas/psr /mnt/nas/
3JThF#95


sudo docker exec -it container_bi_zhanglei_20220627-1504 /bin/bash



# TVM 调优日志搜索
#	autotvm

sudo docker run --name "gpu_tvm_v2_zhanglei" --gpus all --shm-size=1g --ulimit memlock=-1 --ulimit stack=67108864 -it --rm -v /home/lei.zhang/:/home/lei.zhang/ --privileged  tvm_gpu_test_images


from tensorflow.tools.graph_transforms import TransformGraph
BATCH = 8

def export_pb(session):
    # import pdb
    # pdb.set_trace()
    with tf.gfile.GFile("myexportedmodel_{:d}.pb".format(BATCH), "wb") as f:
        inputs = ["input_ids", "input_mask", "segment_ids"] # replace with your input names
        outputs = ["unstack"] # replace with your output names
        graph_def = session.graph.as_graph_def(add_shapes=True)
        graph_def = tf.compat.v1.graph_util.convert_variables_to_constants(session, graph_def, outputs)
        graph_def = TransformGraph(
            graph_def,
            inputs,
            outputs,
            [
                "remove_nodes(op=Identity, op=CheckNumerics, op=StopGradient)",
                "sort_by_execution_order", # sort by execution order after each transform to ensure correct node ordering
                "remove_attribute(attribute_name=_XlaSeparateCompiledGradients)",
                "remove_attribute(attribute_name=_XlaCompile)",
                "remove_attribute(attribute_name=_XlaScope)",
                "sort_by_execution_order",
                "remove_device",
                "sort_by_execution_order",
                "fold_batch_norms",
                "sort_by_execution_order",
                "fold_old_batch_norms",
                "sort_by_execution_order"
            ]
        )
        f.write(graph_def.SerializeToString())
```



[python extract path](https://www.csdn.net/tags/NtTakg2sNDM1OS1ibG9n.html)

[有效筛选github的issue](https://docs.github.com/cn/search-github/searching-on-github/searching-issues-and-pull-requests)

[gitignore 模板](https://github.com/github/gitignore)

![image-20220712224940831](D:\Repo\learning_repo\cmd.assets\image-20220712224940831.png)
