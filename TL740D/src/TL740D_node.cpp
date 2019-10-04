//
// Created by zhao on 19-7-29.
//
#include <string>
#include <ros/ros.h>                           // 包含ROS的头文件
#include <ros/console.h>                        // ROS log
#include <geometry_msgs/Quaternion.h>
#include <eigen3/Eigen/Dense>
// #include <geometry_msgs/Vector3.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <boost/asio.hpp>                  //包含boost库函数
#include <boost/bind.hpp>
#include <math.h>
#include "std_msgs/String.h"              //ros定义的String数据类型
#include <std_msgs/Float32.h>

#include "utils.h"

using namespace std;
using namespace boost::asio;           //定义一个命名空间，用于后面的读写操作

unsigned char buf[34];                      //定义字符串长度，IMU返回的数据是17个字节一组，可用串口调试助手获得

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

float transfor_float(const std::string& input)
{
    float output;
    std:stringstream ss;
    ss << input.substr(1, 6);
    ss >> output;
    // cout << input << endl;
    output = output / 100 * (input.substr(0, 1)=="1"? -1:1);
    return output;
}

int main(int argc, char** argv)
{
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
    // ros::Publisher IMU_pub = n.advertise<std_msgs::Float32>(pub_topic_name, 1000);      //定义发布消息的名称及sulv
    ros::Publisher imu_pub = n.advertise<sensor_msgs::Imu>(pub_topic_name_, 1000);
    ros::Publisher imu_mag_pub = n.advertise<sensor_msgs::MagneticField>(pub_mag_topic_name_, 1000);

    ros::Rate loop_rate(loop_rate_);

    io_service iosev;
    serial_port sp(iosev, USBDevice_);         //定义传输的串口
    sp.set_option(serial_port::baud_rate(baud_rate));
    sp.set_option(serial_port::flow_control());
    sp.set_option(serial_port::parity());
    sp.set_option(serial_port::stop_bits());
    sp.set_option(serial_port::character_size(8));

    std:stringstream ss;
    bool start_flag = true;
    while (ros::ok())
    {
        while(start_flag){
            read(sp,buffer(buf));
            string str(&buf[0],&buf[34]);            //将数组转化为字符串
            string sbuff = string_to_hex(str);
            string::size_type position = sbuff.find("6821");
            ROS_DEBUG_STREAM("positon: "<<position);
            ROS_DEBUG_STREAM("sbuff: "<<sbuff);
            if (position == 0)
            {
                ROS_WARN_STREAM("leave!");
                start_flag = false;
                break;
            }
            if (position != sbuff.npos)
            {
                 int unexpected = int(position)/2;
                read(sp, buffer(buf, unexpected));
                ROS_WARN_STREAM("find! jump: " << unexpected);
            } else{
                ROS_WARN_STREAM("buf not find flag: 6821");
            }
        }
        read(sp,buffer(buf));
        string str(&buf[0],&buf[34]);            //将数组转化为字符串
        string sbuff = string_to_hex(str);
        bool flag = sbuff.substr(0, 8)=="68210084";
        assert(flag);

        float f_roll, f_pitch, f_yaw, f_accx, f_accy, f_accz, f_groyx, f_groyy, f_groyz;
        int KouZCnt, Sum; string s_Status;
        f_roll = transfor_float(sbuff.substr(8, 6));
        f_pitch = transfor_float(sbuff.substr(14, 6));
        f_yaw = transfor_float(sbuff.substr(20, 6));
        f_accx = transfor_float(sbuff.substr(26, 6));
        f_accy = transfor_float(sbuff.substr(32, 6));
        f_accz = transfor_float(sbuff.substr(38, 6));
        f_groyx = transfor_float(sbuff.substr(44, 6));
        f_groyy = transfor_float(sbuff.substr(50, 6));
        f_groyz = transfor_float(sbuff.substr(56, 6));
        ss.str(""); ss << sbuff.substr(62, 6); ss >> KouZCnt;
        s_Status = sbuff.substr(64, 6);
        ss.str(""); ss << sbuff.substr(66, 6); ss >> Sum;

        ROS_DEBUG_STREAM("f_roll:" <<f_roll << "\tf_pitch:" << f_pitch << "\tf_yaw:" << f_yaw
        << "\tf_accx:" << f_accx << "\tf_accy:"<< f_accy<< "\tf_accz:"<< f_accz
        << "\tf_groyx:" << f_groyx << "\tf_groyy:"<< f_groyy<< "\tf_groyz:"<< f_groyz);
        ROS_DEBUG_STREAM(sbuff);


        sensor_msgs::Imu imu_data;
        imu_data.header.stamp = ros::Time::now();
        imu_data.header.frame_id = "imu_link";
        // yaw, pitch, roll --> z, y, x
        Eigen::Vector3f ea0(f_yaw, f_pitch, f_roll);

        Eigen::Quaterniond q =
                Eigen::AngleAxisd(ea0[2], ::Eigen::Vector3d::UnitX()) *
                Eigen::AngleAxisd(ea0[1], ::Eigen::Vector3d::UnitY()) *
                Eigen::AngleAxisd(ea0[0], ::Eigen::Vector3d::UnitZ());

        imu_data.orientation.x = q.x();
        imu_data.orientation.y = q.y();
        imu_data.orientation.z = q.z();
        imu_data.orientation.w = q.z();
        imu_data.angular_velocity.x = f_groyx;
        imu_data.angular_velocity.y = f_groyy;
        imu_data.angular_velocity.z = f_groyz;
        imu_data.linear_acceleration.x = f_accx;
        imu_data.linear_acceleration.y = f_accy;
        imu_data.linear_acceleration.z = f_accz;
        imu_pub.publish(imu_data);

        sensor_msgs::MagneticField imu_mag;
        imu_mag.header = imu_data.header;
        imu_mag.magnetic_field.x = f_roll;
        imu_mag.magnetic_field.y = f_pitch;
        imu_mag.magnetic_field.z = f_yaw;
        imu_mag_pub.publish(imu_mag);
        
        ros::spinOnce();
        loop_rate.sleep();
    }
    iosev.run();
    return 0;
}
