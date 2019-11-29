//
// Created by zhao on 19-2-27.
//

#ifndef ORDERRECEIVER_UTILS_H
#define ORDERRECEIVER_UTILS_H

#include <vector>
#include <ros/ros.h>
#include <ros/console.h>
#include <sstream>
#include <iostream>
#include <queue>
#include <unistd.h>
enum FILE_ERROE 
{
    NO_EXIST=1,
    NO_READ_PERMISSION=2,
    NO_WRITE_PERMISSION=3,
};
using namespace std;

template <typename T>
T readParam(ros::NodeHandle &n, std::string name)
{
    T ans;
    if (n.getParam(name, ans)) {
        ROS_INFO_STREAM("Loaded " << name << ": " << ans);
    }
    else {
        ROS_ERROR_STREAM("Failed to load " << name);
        n.shutdown();
    }
    return ans;
}
/* check file */
int MyCheckFile(const char* file_name){
    if((access(file_name,F_OK))==-1)   
    {   
        printf(" File %s  is not exit,please check again.\n",file_name);
        return FILE_ERROE::NO_EXIST;
    }
    if(access(file_name,R_OK)==-1)   
    {   
        printf("User not have File %s  Read Permission,please check again.\n",file_name);
        return FILE_ERROE::NO_READ_PERMISSION;
    }
  
    if(access(file_name,W_OK)==-1)   
    {   
        printf("User not have File %s  Write Permission,please check again.\n",file_name);
        return FILE_ERROE::NO_WRITE_PERMISSION;
    }   
    return 0;
}
#endif // ORDERRECEIVER_UTILS_H
