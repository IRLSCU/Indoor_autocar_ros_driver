#define ORDER_LENGTH 10
#include <string>
#include <ros/ros.h>                           // 包含ROS的头文件
#include <ros/console.h>                        // ROS log
#include <sstream>
#include <queue>
#include <boost/asio.hpp>                  //包含boost库函数,串口通信
#include <boost/bind.hpp>
#include "std_msgs/UInt8MultiArray.h"

#include "utils.h"

using namespace boost::asio; 

void callback(const std_msgs::UInt8MultiArray::ConstPtr& order);
std::queue<uint8_t> buf;
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

    ros::Subscriber order_receiver_sub = n.subscribe<std_msgs::UInt8MultiArray>(pub_topic_name_,1000,callback);
    ros::Rate loop_rate(loop_rate_);

    io_service iosev;
    serial_port sp(iosev, USBDevice_);         //定义传输的串口
    sp.set_option(serial_port::baud_rate(baud_rate));
    sp.set_option(serial_port::flow_control());
    sp.set_option(serial_port::parity());
    sp.set_option(serial_port::stop_bits());
    sp.set_option(serial_port::character_size(8));
    while (ros::ok())
    {
        uint8_t order[ORDER_LENGTH];  
        if(buf.size()==ORDER_LENGTH){
            std::stringstream ss;
            ss<<"order_receiver_info:";
            for(int i=0;i<ORDER_LENGTH;i++){
                uint8_t num = buf.front();
                ss<<" "<<(int)num;
                order[i]=num;
                buf.pop();
            }
            ROS_INFO_STREAM(ss.str());
            if(order[0]==0xFF&&order[1]==0xFE){//头部校验
                size_t  data_len;       
                data_len = write( sp, buffer(order));
            }
        }else if(buf.size()!=0){
            ROS_INFO_STREAM("[Error]order_receiver_node----------->the data format error,size is buf.size()");
        }
        
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
void callback(const std_msgs::UInt8MultiArray::ConstPtr& order){
    int i=0;
    for(int i=0;i<order->data.size();i++){
        buf.push(order->data[i]);
    }
    return;
}