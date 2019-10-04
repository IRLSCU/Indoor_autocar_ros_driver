//
// Created by zhao on 19-2-27.
//

#ifndef GYUS42_UTILS_H
#define GYUS42_UTILS_H

#include <vector>
#include <ros/ros.h>
#include <ros/console.h>
#include <sstream>
#include <iostream>
#include <queue>

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

#endif // GYUS42_UTILS_H
