
# USB config file

> date: 2019-10-04 11:35:56
> 
> author: wangpengcheng
> 
> e-email: 67318396@qq.com
> 

There some config files and bash file for **devices UBS rule on Linux**。Using these config file ,you can **avoid USB device conflicts**。

## Working principle

### step 1: Query device hardware number

Generally, the USB device is inserted, and the device number of `ttyUSB*` is automatically assigned. You can use `ls -ahl /dev/ttyUSB*` to view all USB interfaces. 

Check your devices number  and use `udevadm` command select your device information. Example:

```bash
udevadm info --attribute-walk --name=/dev/ttyUSB0
```

This result is next :

```bash
Udevadm info starts with the device specified by the devpath and then
walks up the chain of parent devices. It prints for every device
found, all possible attributes in the udev rules key format.
A rule to match, can be composed by the attributes of the device
and the attributes from one single parent device.

  looking at device '/devices/pci0000:00/0000:00:14.0/usb1/1-2/1-2:1.0/ttyUSB0/tty/ttyUSB0':
    KERNEL=="ttyUSB0"
    SUBSYSTEM=="tty"
    DRIVER==""

  looking at parent device '/devices/pci0000:00/0000:00:14.0/usb1/1-2/1-2:1.0/ttyUSB0':
    KERNELS=="ttyUSB0"
    SUBSYSTEMS=="usb-serial"
    DRIVERS=="cp210x"
    ATTRS{port_number}=="0"

  looking at parent device '/devices/pci0000:00/0000:00:14.0/usb1/1-2/1-2:1.0':
    KERNELS=="1-2:1.0"
    SUBSYSTEMS=="usb"
    DRIVERS=="cp210x"
    ATTRS{authorized}=="1"
    ATTRS{bAlternateSetting}==" 0"
    ATTRS{bInterfaceClass}=="ff"
    ATTRS{bInterfaceNumber}=="00"
    ATTRS{bInterfaceProtocol}=="00"
    ATTRS{bInterfaceSubClass}=="00"
    ATTRS{bNumEndpoints}=="02"
    ATTRS{interface}=="CP2104 USB to UART Bridge Controller"
    ATTRS{supports_autosuspend}=="1"

  looking at parent device '/devices/pci0000:00/0000:00:14.0/usb1/1-2':
    KERNELS=="1-2"
    SUBSYSTEMS=="usb"
    DRIVERS=="usb"
    ATTRS{authorized}=="1"
    ATTRS{avoid_reset_quirk}=="0"
    ATTRS{bConfigurationValue}=="1"
    ATTRS{bDeviceClass}=="00"
    ATTRS{bDeviceProtocol}=="00"
    ATTRS{bDeviceSubClass}=="00"
    ATTRS{bMaxPacketSize0}=="64"
    ATTRS{bMaxPower}=="100mA"
    ATTRS{bNumConfigurations}=="1"
    ATTRS{bNumInterfaces}==" 1"
    ATTRS{bcdDevice}=="0100"
    ATTRS{bmAttributes}=="80"
    ATTRS{busnum}=="1"
    ATTRS{configuration}==""
    ATTRS{devnum}=="69"
    ATTRS{devpath}=="2"
    ATTRS{idProduct}=="ea60"
    ATTRS{idVendor}=="10c4"
    ATTRS{ltm_capable}=="no"
    ATTRS{manufacturer}=="Silicon Labs"
    ATTRS{maxchild}=="0"
    ATTRS{product}=="CP2104 USB to UART Bridge Controller"
    ATTRS{quirks}=="0x0"
    ATTRS{removable}=="unknown"
    ATTRS{serial}=="01BBF529"
    ATTRS{speed}=="12"
    ATTRS{urbnum}=="18"
    ATTRS{version}==" 2.00"

  looking at parent device '/devices/pci0000:00/0000:00:14.0/usb1':
    KERNELS=="usb1"
    SUBSYSTEMS=="usb"
    DRIVERS=="usb"
    ATTRS{authorized}=="1"
    ATTRS{authorized_default}=="1"
    ATTRS{avoid_reset_quirk}=="0"
    ATTRS{bConfigurationValue}=="1"
    ATTRS{bDeviceClass}=="09"
    ATTRS{bDeviceProtocol}=="01"
    ATTRS{bDeviceSubClass}=="00"
    ATTRS{bMaxPacketSize0}=="64"
    ATTRS{bMaxPower}=="0mA"
    ATTRS{bNumConfigurations}=="1"
    ATTRS{bNumInterfaces}==" 1"
    ATTRS{bcdDevice}=="0415"
    ATTRS{bmAttributes}=="e0"
    ATTRS{busnum}=="1"
    ATTRS{configuration}==""
    ATTRS{devnum}=="1"
    ATTRS{devpath}=="0"
    ATTRS{idProduct}=="0002"
    ATTRS{idVendor}=="1d6b"
    ATTRS{interface_authorized_default}=="1"
    ATTRS{ltm_capable}=="no"
    ATTRS{manufacturer}=="Linux 4.15.0-50-generic xhci-hcd"
    ATTRS{maxchild}=="10"
    ATTRS{product}=="xHCI Host Controller"
    ATTRS{quirks}=="0x0"
    ATTRS{removable}=="unknown"
    ATTRS{serial}=="0000:00:14.0"
    ATTRS{speed}=="480"
    ATTRS{urbnum}=="1184"
    ATTRS{version}==" 2.00"

  looking at parent device '/devices/pci0000:00/0000:00:14.0':
    KERNELS=="0000:00:14.0"
    SUBSYSTEMS=="pci"
    DRIVERS=="xhci_hcd"
    ATTRS{broken_parity_status}=="0"
    ATTRS{class}=="0x0c0330"
    ATTRS{consistent_dma_mask_bits}=="64"
    ATTRS{d3cold_allowed}=="1"
    ATTRS{dbc}=="disabled"
    ATTRS{device}=="0xa12f"
    ATTRS{dma_mask_bits}=="64"
    ATTRS{driver_override}=="(null)"
    ATTRS{enable}=="1"
    ATTRS{irq}=="124"
    ATTRS{local_cpulist}=="0-3"
    ATTRS{local_cpus}=="f"
    ATTRS{msi_bus}=="1"
    ATTRS{numa_node}=="-1"
    ATTRS{revision}=="0x31"
    ATTRS{subsystem_device}=="0x06f7"
    ATTRS{subsystem_vendor}=="0x1028"
    ATTRS{vendor}=="0x8086"

  looking at parent device '/devices/pci0000:00':
    KERNELS=="pci0000:00"
    SUBSYSTEMS==""
    DRIVERS==""

```


Qízhōng `ATTRS{idProduct}`hé `ATTRS{idVendor`dàibiǎo shèbèi de yìngjiàn xīnpiàn xìnxī. Shì cāozuò xìtǒng nèihé shìbié shèbèi de guānjiàn.
64/5000
Where `ATTRS{idProduct}` and `ATTRS{idVendor` represent the hardware chip information of the device. It is the key to the operating system kernel identification device.

you can also use `lsusb` command to get it.

## Step 2： Change the `udev rules` rules file

This is a udev rubles file in this folder for example:

```bash

# ultrasonic_radar usb rules
KERNEL=="ttyUSB*", SUBSYSTEMS=="usb", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", GROUP="users", MODE="0666", ENV{ID_MM_DEVICE_IGNORE}="1", SYMLINK+="ultrasonic_radar"
# imu usb rules
KERNEL=="ttyUSB*", SUBSYSTEMS=="usb", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001", GROUP="users", MODE="0666", ENV{ID_MM_DEVICE_IGNORE}="1", SYMLINK+="imu"
```

You should add your device‘s `ATTRS{idProduct}` and `ATTRS{idVendor`。And give your device other name by set `SYMLINK+="imu"`，change permission by `MODE="0666"`。

### step 3: restart udev

You can run it with this `restart_rules.sh`

```bash

sudo bash restart_rules.sh
```

or by yourself:

```bash
# cpoy rule file to your rules folder

sudo cp ./com_port.rules /etc/udev/rules.d/
#restart udev service

sudo service udev reload
sudo service udev restart

```

Notce:

**Here, the configuration according to the device chip class is unique. One rule can only be configured in the device. When the two devices are the same device, the device chip cannot be distinguished.**

You can get more information form here: [https://wangpengcheng.github.io/2019/09/18/change_usb_rules_ubuntu/](https://wangpengcheng.github.io/2019/09/18/change_usb_rules_ubuntu/)


# Set frp

we use frp to get ssh function in public network;You should install frp to your computer and set it self start

## Step 1: Install frp Service and client
You can get information form next link:

- [处于不同局域网下的Socket通信——frp配置](https://blog.csdn.net/m0_37817028/article/details/89468704)
- [frp help](https://github.com/fatedier/frp/)

## Step 2: Change the config file

set the config file `frpc.ini`
you should chang your serivce ip address and port number

change the `frp.conf` file. Set frpc path to youself. 
```
command =/home/nvidia/frp/frpc -c /home/nvidia/frp/frpc.ini
```

## Step 3: Auto setup

you can run the `config.sh` to auto config it.

You can set it yourself

```shell
sudo apt-get -y install supervisor
sudo cp ./frp.conf /etc/supervisor/conf.d/frp.conf
# 重启supervisor
sudo systemctl restart supervisor
#重启frp服务
sudo supervisorctl restart frp
```
