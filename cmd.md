# CMD



```bash
poweruser
9d@n2N3Vt5Z6

sudo groupadd docker
sudo usermod -aG docker ${USER}
sudo systemctl restart docker

dd if=/dev/zero of=test.bin bs=1k count=1000

export http_proxy=http://192.168.100.200:3128
export https_proxy=http://192.168.100.200:3128

apt update && apt install -y sshfs
yum clean all && yum update
yum install epel-release
yum install fuse-sshfs 
sshfs -o allow_root,default_permissions root@10.113.3.3:/data /mnt/fileserver/
corex@2018
sshfs -o allow_root,default_permissions root@10.113.1.9:/mnt/nas/psr /mnt/nas


ln -s  /mnt/nas/Home/lei.zhang/remote_src remote_src

mount -t nfs 10.113.1.9:/mnt/nas/psr /mnt/nas
mount -t nfs 10.113.3.3:/data /mnt/nas

docker login -u lei.zhang1 10.150.9.98:80
# ping 工具安装
apt-get install inetutils-ping

# git 忽略权限配置
git config --global core.filemode false
git config  core.filemode false
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
git config --global user.email "lei.zhang1@iluvatar.com"

# 统一在自己项目目录使用--local参数：
git config --local user.name "lei.zhang1"
git config --local user.email "lei.zhang1@iluvatar.com"

git config --global user.name "chenugray"
git config --global user.email "chenugray215@outlook.com"

# docker
docker run -it --name igie_zhanglei_0513 --privileged --cap-add=ALL -v /dev:/dev -v /root:/root -v /lib/modules:/lib/modules -v /usr/src:/usr/src -v /home/lei.zhang:/home/lei.zhang 10.150.9.98:80/sw_test/apps:2.3.0.20221115.265-centos-7.8.2003-x86_64-10.2-python3.7_mr /bin/bash

docker run --name "container_bi_20220422" -it --privileged --pid=host --cap-add=ALL -v /dev:/dev -v /lib/modules:/lib/modules -v /usr/src:/usr/src -v /home/lei.zhang:/home/lei.zhang container_bi_20220422-1130 /bin/bash

docker run --name "tvm0.9dev_leizhang" -it --privileged --pid=host --cap-add=ALL -v /dev:/dev -v /lib/modules:/lib/modules -v /usr/src:/usr/src -v /home/lei.zhang:/home/lei.zhang 2912baf2ad1 /bin/bash

docker run --name "bi_img_leizhang_20220210" -it --privileged --pid=host --cap-add=ALL -v /dev:/dev -v /lib/modules:/lib/modules -v /usr/src:/usr/src -v /home/lei.zhang:/home/lei.zhang -v /mnt/nas/home/lei.zhang/:/mnt/nas/home/lei.zhang/ 10.150.9.98:80/performance/bi_img:20220210 /bin/bash

docker run --name "bi_img_leizhang_c" -it --privileged --pid=host --cap-add=ALL -v /dev:/dev -v /lib/modules:/lib/modules -v /usr/src:/usr/src -v /home/lei.zhang:/home/lei.zhang  container_bi_20220422-1130:1.0 /bin/bash

sudo docker run --name "tvm0.9dev_leizhang_21.10" --shm-size=16g --ulimit --ulimit stack=67108864 -it -v /home/lei.zhang/:/home/lei.zhang/ --privileged  nvcr.io/nvidia/tensorrt:21.10-py3

docker run --name "mr_0810_latest" -it --privileged --pid=host --cap-add=ALL -v /dev:/dev -v /lib/modules:/lib/modules -v /usr/src:/usr/src -v /home/lei.zhang:/home/lei.zhang  10.150.9.98:80/sw_test/apps:mr_alpha_20220809-centos-7.8.2003-x86_64-10.2-python3.7_mr /bin/bash


## docker 拉起pytorch gpu镜像
sudo nvidia-docker run  --name "zhanglei.cuda11.7_llama" --ulimit memlock=-1 --ulimit stack=67108864 -it  -v /home/lei.zhang/:/home/lei.zhang/ -v /mnt/:/mnt/ -v /data0:/data0 -v /data:/data  --privileged  nvcr.io/nvidia/cuda:11.7.1-cudnn8-devel-ubuntu20.04

sudo nvidia-docker run --gpus all --shm-size=16g --ulimit memlock=-1 --ulimit stack=67108864 -it -v /home/lei.zhang/:/home/lei.zhang/ --privileged  nvcr.io/nvidia/tensorrt:22.02-py3

docker run --name "lei.zhang230215" -it --privileged --pid=host --cap-add=ALL -v /dev:/dev -v /lib/modules:/lib/modules -v /usr/src:/usr/src -v /home/lei.zhang:/home/lei.zhang -v /mnt/nas/home/lei.zhang/remote_src/:/home/lei.zhang/src_code/remote_src/ 10.150.9.98:80/sw_test/apps@sha256:381596743f46e4b13b41c7b891f658d985b81d664758d9cc698f5905be93c2cc /bin/bash

sudo docker exec -it lei.igie_zhanglei_0513 /bin/bash
sudo docker exec -it distracted_pasteur /bin/bash
bi_img_leizhang_20220217
sudo docker exec -it bi_img_leizhang_20220217 /bin/bash
sudo docker exec -it container_leizhang_20220606-1756 /bin/bash
container_bi_20220422-1130
sudo docker exec -it container_bi_20220422-1130 /bin/bash
sudo docker exec -it zhanglei_tensorrt_22.02 /bin/bash
sudo docker exec -it mr_0810_latest /bin/bash
reverent_lamarr
# 将运行的镜像保存为可加载镜像，以便后续拉起
sudo docker commit 14478deb892d tvm_gpu_test_images
sudo docker run --name "tf1_py3_lei_zhang" --gpus all --shm-size=1g --ulimit memlock=-1 --ulimit stack=67108864 -it --rm -v /home/lei.zhang/:/home/lei.zhang/ -v /mnt/nas/:/mnt/nas/ --privileged  nvcr.io/nvidia/tensorflow:22.07-tf1-py3

sudo -s
# adduser user.name --force-badname

useradd  -m -d /home/lei.zhang -s /bin/bash lei.zhang
usermod -a -G sudo fanwu.han

# 格式化时间
date "+%Y_%m_%d %H:%M:%S"  # 2022_03_11 10:00:13
date "+%Y_%m_%d_%H_%M" # 2022_03_11_10_01


MR的1.0.5固件可以从以下位置获取：
/mnt/nas/home/yuanpei.wang/tools/MR/FW/1.0.5/mr50_75w_v1.0.5
/mnt/nas/home/yuanpei.wang/tools/MR/FW/1.0.5/mr100_150w_v1.0.5

使用方法如下：
./ixfw-tool -f mr50_75w_v1.0.5.hex
./ixfw-tool -f mr100_150w_v1.0.5.hex

10.150.9.98:80/sw_test/apps@sha256:043ac7e87013bff65e6f17f18e7c9756282970df32da6983543f091b34f0dedb

docker pull 10.150.9.98:80/sw_test/apps:latest-centos-7.8.2003-x86_64-10.2-python3.6_mr


sudo docker run --name "lei.zhang_0308" -it --privileged --pid=host --cap-add=ALL -v /dev:/dev -v /lib/modules:/lib/modules -v /usr/src:/usr/src -v /home/lei.zhang:/home/lei.zhang 10.150.9.98:80/sw_test/apps@sha256:043ac7e87013bff65e6f17f18e7c9756282970df32da6983543f091b34f0dedb  /bin/bash


sudo docker run --name "gpu_pytorch_v6_zhanglei" --gpus all --shm-size=1g --ulimit memlock=-1 --ulimit stack=67108864 -it -v /home/lei.zhang/:/home/lei.zhang/ --privileged  cmcc:conformer

sudo docker exec -it lei.igie_zhanglei_0513 /bin/bash
ssh lei.zhang1@fanwu.han@10.113.1.30@10.101.1.125 -p 2222
ssh lei.zhang1@fanwu.han@10.113.1.30@10.101.1.125 -p 2222
ssh lei.zhang1@lei.zhang@10.201.40.28@10.101.1.125 -p 2222

10.113.3.3
root
corex@2018

source /opt/rh/devtoolset-9/enable
```






$$
l\_shift = s > 0 ? s : 0\\
r\_shift = s > 0 ? 0 : -s\\


z = \frac{x * 2^{l\_shift} * y + 2^{r\_shift + q - 1}}{2^{r\_shift+q}} = x * y * 2^{l\_shift - r\_shift - q} + 2 ^ {-1} \\
for\ any \ s \ always \ has \ (l\_shift - r\_shift) = s \\
z = x * y * 2 ^ {s - q} + 0.5
\\
scale = y * 2 ^ {s - q} 
\\
z = round(x * scale + 0.5)
$$


# Transformer Translation en2fr

  

该模型是transformer结构的encoder+decoder模型进行translation任务，具体来源如下

  

> fairseq translation en2fr example: https://github.com/facebookresearch/fairseq/tree/main/examples/translation

  

当前推荐使用64batch和128batch进行推理，使用其他batch可能会产生性能退化现象, 这里给出参考8/16/32/64/128 batch 参考性能。

  

## result


| Column 1 | Column 2 |
| -------- | -------- |
| Row 1, Column 1 | Row 1, Column 2 |
| Row 2, Column 1 | Row 2, Column 2 |

  

|| batch_size   || performance      || accuracy(BLUE4)  ||
| ----          |  ------------     | -----------       |
| 8             | 9.32 sentences/s  | 42.34             |
| 16            | 15.71 sentences/s | 42.34             |
| 32            | 24.31 sentences/s | 42.34             |
| 64            |265.17 sentences/s | 42.34             |
| 128           |301.87 sentences/s | 42.34             |