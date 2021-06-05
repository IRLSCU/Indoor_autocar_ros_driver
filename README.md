# Indoor auto car devices driver package

## Abstract

This project is a part of Indoor auto car project. It main contain those device drivers code by ROS on NVIDIA Jetson TX2. It mainly contains the ROS_NODE code for various devices. The main role is to connect devices, send device information and accept control.

## File structure description

Most folders is named by device. The device driver folder name can be found in the device list below. The config file folder contain device config file and bash file. The Documentation folder contains all device driver related documentation and examples.

## System and Software

- Ubuntu 16.04 on Jetson TX2
- Ros kinetic base;

## Devices list

- rplidar_ros:Single line laser radar ros drive，source code from [rplidar_ros](https://github.com/Slamtec/rplidar_ros.git);Get more information from here: [prlidar A1](http://www.slamtec.com/cn/Lidar/A1)
- ros_rslidar:The 16-beam LIDAR is not used in indoor cars for the time being.You can get more information in the documentation folder.
- GYUS42:Ultrasonic radar, mainly for short distance obstacle ranging, sending obstacle distance information.You can get more information in the documentation.
- TL740D: IMU device driver source code;You can get more information in the documentation.
- usb_cam:A ROS Driver for V4L USB Cameras;source code can be found here:[usb_cam](https://github.com/bosch-ros-pkg/usb_cam.git).
- MYNT-EYE-S-SDK: A ROS MYNT-EYE Binocular camera driver, source code can be found here:[MYNT-EYE-S-SDK](https://github.com/slightech/MYNT-EYE-S-SDK.git).
- order_receiver: A indoor car 80cm*50cm controll code. 
- wheel_speed: wheel speed count node from car driver.

## TODO


- [x] Car control driver
- [ ] Intel Neural Compute Stick2 driver
- [x] Binocular camera driver


## Quick start

**Notice: This project is based ROS,YOU MUST INSTALL ROS BEFORE**.You can get more information form here:[Ros](https://www.ros.org/)

### Build

you can clone this project in your ROS workspace. clone it and build it by `catkin_make`. Below is a simple example

```bash
cd ~/catkin_ws
cd src/
git clone https://github.com/wangpengcheng/Indoor_autocar_ros_driver.git
# or get submodule
git clone --recurse-submodules https://github.com/wangpengcheng/Indoor_autocar_ros_driver.git

cd ..
catkin_make

```
### Device config

For devices config,you should run `config.sh` in config_file folder.Like this:

```bash

sudo bash ~/catkin_ws/src/config_file/config.sh

```
You can install `cutecom` to test those USB devices.

You can get more information from [./config_file/readme.md](./config_file/readme.md);



### Run

First start your ROSMASTER whith `roscore`,next setup ROS  environment
```bash
cd ~/catkin_ws
source ./devel/setup.bash
```
all ros driver has  launch file to Quick run,and you can use it for quick start.For Example:

```bash
#start GYUS42
roslaunch GYUS42 GYUS42.launch
#start roslaunch 
roslaunch TL740D TL740D.launch
#start rplidar_ros
roslaunch rplidar_ros rplidar.launch
```
## GetDocumentation And Submodule

You can use those command update all submodule
```bash
git submodule updata --init --recursive
```
or update farther submodule
```bash
git pull
git submodule update
```

## Contributors 

- [WangPengCheng](https://github.com/wangpengcheng/)
- [real rui](https://github.com/HuangRui001)
- [zhaostu4](https://github.com/zhaostu4)


