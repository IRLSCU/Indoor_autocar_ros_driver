/****************************************************************************
** @projectName   
** Version    
** This file is part of the  project.
** @file          .cpp
** @brief         GYUS42 串口通信
** @details       超声波雷达的串口通信方法，并且通过ROS节点进行topic的发布
** @author        wangpengcheng
** @QQ            673018396
** @email         wangpengcheng2018@gmail.com
** @address       
** @date          2019-10-3 周三  17:06:21
** @example
**
** 示例代码
**
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>

#ifdef USE_ROS
/* ------ include ros start ----- */
#include <string>
#include <iostream>
#include <ros/ros.h>                           // 包含ROS的头文件
#include <ros/console.h>                        // ROS log
#include <geometry_msgs/Quaternion.h>
/* ------ include ros end ----- */
/* ------ include msg and string start -----*/
#include "std_msgs/Int16.h"
#include "std_msgs/Header.h"
#include "std_msgs/String.h"              //ros定义的String数据类型
#include <std_msgs/Float32.h>
#include "utils.h"

/* ------ include msg and string end -----*/
using namespace std;
//using namespace boost::asio;           //定义一个命名空间，用于后面的读写操作

#endif //ros
int set_opt(int,int,int,char,int);
std::string string_to_hex(const std::string& input);
int open_port(char* uartname)
{
    int fd = open(uartname, O_RDWR|O_NONBLOCK);
    if (-1 == fd)
    {
        perror("Can't Open Serial Port");
        return(-1);
    }
     /*恢复串口为阻塞状态*/
     if(fcntl(fd, F_SETFL, 0)<0)
     {
            printf("fcntl failed!\n");
     }else{
        printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
     }
     /*测试是否为终端设备*/
     if(isatty(STDIN_FILENO)==0)
     {
        printf("standard input is not a terminal device\n");
     }
     else
     {
        printf("isatty success!\n");
     }
     printf("fd-open=%d\n",fd);
     return fd;
}
int main(int argc, char* argv[])
{
/*----- with ROS start ------ */
#ifdef USE_ROS
    //ROS 接收参数和初始化
    ros::init(argc, argv, "TL740D");       //初始化节点
    ros::NodeHandle n("~");
    string pub_topic_name_, pub_mag_topic_name_, USBDevice_;
    int loop_rate_, baud_rate;
    n.getParam("pub_topic_name", pub_topic_name_);
    n.getParam("pub_mag_topic_name", pub_mag_topic_name_);
    n.getParam("serial_device", USBDevice_);
    n.getParam("loop_rate", loop_rate_);
    n.getParam("baud_rate", baud_rate);

    ROS_WARN_STREAM("pub_topic_name_:" << pub_topic_name_);
    ROS_WARN_STREAM("USBDevice_:" << USBDevice_);
    ROS_WARN_STREAM("loop_rate_:" << loop_rate_);
    ROS_WARN_STREAM("baud_rate:" << baud_rate);
    //get message
    ros::Publisher ultrasonic_pub = n.advertise<std_msgs::Header>(pub_topic_name_, 10);
    ros::Rate loop_rate(loop_rate_);
/*
    
    io_service iosev;
    serial_port sp(iosev, USBDevice_);         //定义传输的串口
    sp.set_option(serial_port::baud_rate(baud_rate));
    sp.set_option(serial_port::flow_control());
    sp.set_option(serial_port::parity());
    sp.set_option(serial_port::stop_bits());
    sp.set_option(serial_port::character_size(8));
*/
#endif
/*----- with ROS end ------ */
    int fd=1, read_num = 0;
    unsigned char buffer[30];
    memset(buffer,0,30);
   //设置默认USB接口编号
    char* uartname="/dev/ultrasonic_radar";
#ifdef USE_ROS
    if(!USBDevice_.empty()){
        int len=USBDevice_.size();
        uartname=(char *)malloc((len+1)*sizeof(char));
        memset(uartname,0,len+1);
        strcpy(uartname,USBDevice_.c_str());
    }
#endif
//    if(argc < 2)
//    {
//        printf("usage: ./uarttest /dev/ttyUSB0 \n");
//        return 0;
//    }
    if((fd=open_port(uartname))<0)
    {
         printf("open %s is failed\n",uartname);
         return 0;
    }
    else{
            set_opt(fd, 9600, 8, 'N', 1);
            printf("set_opt fd=%d\n",fd);
            char is_distance=0x45;
#ifdef USE_ROS
            while(ros::ok()){
#else
            while(1){
#endif
                    // char c=getchar();
                    // printf("%c",c);
                    // if('q'==c)
                    // {
                    //     return 0;
                    // }
#ifdef USE_ROS
                    loop_rate.sleep();
                    //睡眠一定时间，等待数据写入，时间间隔为000ms,0.5s
#else
                    usleep(500000);
#endif
                    memset(buffer, 0, 30);
                    read_num = read(fd, buffer, 30);
#ifdef USE_ROS
                    // string temp_str(&buffer[0],&buffer[34]);
                    // string sbuff = string_to_hex(temp_str);
                    // string::size_type position = temp_str.find("45");
#else
                    printf("read_num=%d \n",read_num);
#endif
                    if(read_num>0){
                        if(buffer[2]==is_distance){
                            unsigned long distance=0;
                            distance=(buffer[4]<<8)|buffer[5];
                        if(distance<=0){
                            printf("error ultrasonic distance: %ld (cm) \n",distance);
                            continue;
                        }
#ifdef USE_ROS
                    //ROS_DEBUG_STREAM("ultrasonic distance:"<<std::to_string(distance));                           
                    std_msgs::Header ultrasonic_data;
                    ultrasonic_data.stamp=ros::Time::now();
                    ultrasonic_data.frame_id="GYUS42";
                    //ultrasonic_data.header.stamp=ros::Time::now();
                    //ultrasonic_data.header.frame_id = "GYUS42";
                    ultrasonic_data.seq=distance;
                    ultrasonic_pub.publish(ultrasonic_data);
                    printf("ultrasonic distance: %ld (cm) \n",distance);
                    ros::spinOnce();
#else
                    
#endif
                        }
                    }else{
#ifdef USE_ROS
                        ROS_WARN_STREAM("read error\n");
#else
                        printf("read error\n");
#endif
                    }
            }
        fd=close(fd);
    }
    free(uartname);
    return 0;
}


int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    if  ( tcgetattr( fd,&oldtio)  !=  0) {
        perror("SetupSerial 1");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch( nBits )
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }

    switch( nEvent )
    {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':
        newtio.c_cflag &= ~PARENB;
        break;
    }

    switch( nSpeed )
    {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    case 460800:
        cfsetispeed(&newtio, B460800);
        cfsetospeed(&newtio, B460800);
        break;
    case 921600:
        printf("B921600\n");
        cfsetispeed(&newtio, B921600);
                cfsetospeed(&newtio, B921600);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }
    if( nStop == 1 )
        newtio.c_cflag &=  ~CSTOPB;
    else if ( nStop == 2 )
    newtio.c_cflag |=  CSTOPB;
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }
  //printf("set done!\n\r");
    return 0;
}

std::string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}