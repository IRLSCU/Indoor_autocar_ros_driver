# ROS MASTER 通信设置

ROS的master通信设置主要分为三步


## 1. 更改hosts 设置


首先需要更改主机电脑和master电脑上的host配置。方便后面的ROS找到对应的IP地址

从机hosts只需要添加master节点的地址和 以及自己的地址和别名

从机  编辑`/etc/hosts`文件，添加如下代码

```bash
192.168.37.252  nvidia # 添加master主机NVIDIA 节点地址
192.168.58.143  wangpengcheng # 添加从机本地IP地址和别名这个会在下一步用到

```

## 2. 更改ROS相关配置

在~/.bashrc 文件中添加如下内容，或者使用文件夹中的`ROS_MASTER.sh`,主要设置内容如下：

```bash
export ROS_HOSTNAME=wangpengcheng #设置从机节点名称，必须和host中设置的别名相同
export ROS_MASTER_URI=nvidia #这里设置主机master节点的别名
```

## 3. 在master主机节点添加所有host的ip别名

和从机不同，master主机需要知道所有节点的名称和ip地址。

在master主机中进行hosts相关配置添加所有从机别名和ip。否则从机发送消息；master无法接收

```bash
192.168.58.143 wangpengcheng # 从机1 ip和节点名称

192.168.62.130 huangrui # 从机2 ip和节点名称

``` 

当出现错误时，记得打印相关环境变量查看，设置是否生效。
