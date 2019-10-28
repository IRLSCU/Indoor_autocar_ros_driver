# 使用nmcli来进行wifi配置和选择

____参考链接：_ 

- [nmcli 命令行方式连接wifi及删除连接操作](https://blog.csdn.net/shaozuo133/article/details/79060536)
- [Terminal下的无线管理工具之Nmcli](http://www.secist.com/archives/1378.html)
- [nmcli 命令行方式连接wifi及删除连接操作](https://blog.csdn.net/shaozuo133/article/details/79060536)



## 1 查看可用wifi
可以使用`sudo nmcli device wifi list`命令查看可用wifi;显示结果如下:

```bash
wangpengcheng@wangpengcheng-Inspiron-3650:~$ sudo nmcli dev wifi
[sudo] wangpengcheng 的密码： 
*   SSID                 模式  频段  速率       信号  BARS  安全性      
    FeiXingMoNin         红外  1     54 Mbit/s  100   ▂▄▆█              
    SCUNET－VVIP         红外  1     54 Mbit/s  100   ▂▄▆█  WPA2        
    FeiXingMoNin         红外  1     54 Mbit/s  100   ▂▄▆█              
    FXMN-VIP             红外  1     54 Mbit/s  100   ▂▄▆█              
    fyhahahah            红外  1     54 Mbit/s  100   ▂▄▆█  WPA2        
    rover                红外  1     54 Mbit/s  100   ▂▄▆█  WPA1        
    FXMN-VIP             红外  6     54 Mbit/s  100   ▂▄▆█              
    B-LINK_92BF5A        红外  10    54 Mbit/s  100   ▂▄▆█  WPA1 WPA2   
    FeiXingMoNin         红外  11    54 Mbit/s  100   ▂▄▆█              
    FXMN-VIP             红外  1     54 Mbit/s  99    ▂▄▆█              
    Boil                 红外  6     54 Mbit/s  97    ▂▄▆█  WPA2        
    eduroam              红外  1     54 Mbit/s  95    ▂▄▆█  WPA2 802.1X 
    FXMN-VIP             红外  11    54 Mbit/s  95    ▂▄▆█              
    ......
```
**注意:使用前先使用命令`nmcli nm wifi off/on`关闭或者开启所有wifi**

## 2 使用命令链接wifi

可以使用如下的命令直接链接已经保存过密码的wifi
```bash
#连接/断开wifi
nmcli connect up/down FeiXingMoNin 
```

第一次连接使用可以使用如下的命令添加对应的wifi信息

```
nmcli connect up rover password 123456 wlan0

```

## 3 查看修改的wifi信息
相关wifi信息保存在`/etc/NetworkManager/system-connections`中;可以查看wifi名称对应的文件。获取更加详细的信息；也可以进行修改；样例文件内容如下：

```
[connection]
id=rover
uuid=bfeabc9d-88dc-4cba-b70a-4f978bd01b2a
type=wifi
permissions=
secondaries=

[wifi]
mac-address=00:04:4B:C4:7F:FF
mac-address-blacklist=
mac-address-randomization=0
mode=infrastructure
seen-bssids=
ssid=rover

[wifi-security]
auth-alg=open
group=
key-mgmt=wpa-psk
pairwise=
proto=
psk=tiansilab

[ipv4]
dns-search=
method=auto

[ipv6]
addr-gen-mode=stable-privacy
dns-search=
method=auto


```


