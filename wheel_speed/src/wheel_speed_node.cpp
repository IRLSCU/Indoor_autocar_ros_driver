
#include <string>
#include <ros/ros.h>                           // 包含ROS的头文件
#include <ros/console.h>                        // ROS log
#include <sstream>
#include <queue>
#include <boost/asio.hpp>                  //包含boost库函数,串口通信
#include <boost/bind.hpp>
#include "geometry_msgs/QuaternionStamped.h"
#include "utils.h"
#include <sys/time.h>
#include <unistd.h>


#define LENGTH 12
using namespace boost::asio;
char feedback_arr[LENGTH];
int findSignal(char* feedback_arr);
int main(int argc, char** argv)
{
    ros::init(argc, argv, "wheel_speed_node");       //初始化节点
    ros::NodeHandle n("~");
    std::string pub_topic_name_, pub_mag_topic_name_, USBDevice_;
    int loop_rate_, baud_rate;
    double wheel_radius;
    n.getParam("pub_topic_name", pub_topic_name_);
    n.getParam("serial_device", USBDevice_);
    n.getParam("loop_rate", loop_rate_);
    n.getParam("baud_rate", baud_rate);
    n.getParam("wheel_radius", wheel_radius);
    ros::Publisher order_feedback_pub = n.advertise<geometry_msgs::QuaternionStamped>(pub_topic_name_,1000);
    ros::Rate loop_rate(loop_rate_);
    /* check file */
   
    int isFileExit=MyCheckFile(USBDevice_.c_str());
    if(isFileExit!=0){
        return 0;
    }
    io_service iosev;
    serial_port sp(iosev, USBDevice_);         //定义传输的串口
    sp.set_option(serial_port::baud_rate(baud_rate));
    sp.set_option(serial_port::flow_control());
    sp.set_option(serial_port::parity());
    sp.set_option(serial_port::stop_bits());
    sp.set_option(serial_port::character_size(8));
    geometry_msgs::QuaternionStamped feedback;
    bool start_flag=false;
    while (ros::ok())
    {
        read(sp,buffer(feedback_arr));
        // for(int i=0;i<LENGTH;i++){
        //    std::cout<<(int)(uint8_t)(feedback_arr[i])<<" ";
        // }
        // std::cout<<std::endl;
        while(!start_flag){
            int index=findSignal(feedback_arr);
            if(index==0){
                start_flag=true;
            }else{
                if(index!=-1){
                    read(sp,buffer(feedback_arr,index));
                    ROS_DEBUG_STREAM("signal in mid "<<index);
                }else{
                    read(sp,buffer(feedback_arr,LENGTH-1));
                    std::cout<<"signal can not be found"<<std::endl;
                }
                break;
            }
        }
        if(start_flag&&((uint8_t)feedback_arr[0])==0xFF&&((uint8_t)feedback_arr[1])==0xFE){//校验
            int rightSpeed=(double)(uint8_t)(feedback_arr[2])*100.0/7000*wheel_radius;
            int rightOrientation=(int)(uint8_t)(feedback_arr[3])==2?0:1;
            int leftSpeed=(double)(uint8_t)(feedback_arr[4])*100.0/7000*wheel_radius;
            int leftOrientation=(int)(uint8_t)(feedback_arr[5])==2?0:1;
            feedback.quaternion.x=leftOrientation;
            feedback.quaternion.w=rightOrientation;
            feedback.quaternion.y=leftSpeed;
            feedback.quaternion.z=rightSpeed;
            order_feedback_pub.publish(feedback);
            //std::cout<<"start boarding feedback"<<std::endl;
            // ros::spinOnce();
            // loop_rate.sleep();
        }else{
            start_flag=false;
            ROS_DEBUG_STREAM("order disorder!!!!");
        }
    }
    //等待处理队伍完成和继续下一步
    iosev.run();
    return 0;
}
int findSignal(char* feedback_arr){
    for(int i=0;i<LENGTH;i++){
        if(((uint8_t)feedback_arr[i])==0xFF){
            if(i+1>=LENGTH){
                return -1;
                std::cout<<"";
            }
            else if(((uint8_t)feedback_arr[i+1])==0xFE){
                return i;
            }
        }
    }
    return -1;
}