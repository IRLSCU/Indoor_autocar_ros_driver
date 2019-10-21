#define ORDER_LENGTH 10
#include <string>
#include <ros/ros.h>                           // 包含ROS的头文件
#include <ros/console.h>                        // ROS log
#include <sstream>
#include <queue>
#include <boost/asio.hpp>                  //包含boost库函数,串口通信
#include <boost/bind.hpp>
#include "geometry_msgs/QuaternionStamped.h"
#include "utils.h"

using namespace boost::asio; 

void callback(const geometry_msgs::QuaternionStamped::ConstPtr& order);
uint8_t order_arr[ORDER_LENGTH];
int main(int argc, char** argv)
{
    ros::init(argc, argv, "order_receiver_node");       //初始化节点
    ros::NodeHandle n("~");
    std::string pub_topic_name_, pub_mag_topic_name_, USBDevice_;
    int loop_rate_, baud_rate;
    n.getParam("pub_topic_name", pub_topic_name_);
    n.getParam("serial_device", USBDevice_);
    n.getParam("loop_rate", loop_rate_);
    n.getParam("baud_rate", baud_rate);

    ros::Subscriber order_receiver_sub = n.subscribe<geometry_msgs::QuaternionStamped>(pub_topic_name_,1000,callback);
    ros::Rate loop_rate(loop_rate_);

    io_service iosev;
    serial_port sp(iosev, USBDevice_);         //定义传输的串口
    sp.set_option(serial_port::baud_rate(baud_rate));
    sp.set_option(serial_port::flow_control());
    sp.set_option(serial_port::parity());
    sp.set_option(serial_port::stop_bits());
    sp.set_option(serial_port::character_size(8));

    order_arr[0]=0xFF;
    order_arr[1]=0xFE;
    while (ros::ok())
    {
        int data_len = write( sp, buffer(order_arr));
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
void callback(const geometry_msgs::QuaternionStamped::ConstPtr& order){
    int i=0;
    int leftOrientation=order->quaternion.x;
    int rightOrientation=order->quaternion.w;
    int leftSpeed=order->quaternion.y;
    int rightSpeed=order->quaternion.z;
    order_arr[3]=leftSpeed;
    order_arr[2]=rightSpeed;
    order_arr[5]=leftOrientation;
    order_arr[4]=rightOrientation;
    return;
}
