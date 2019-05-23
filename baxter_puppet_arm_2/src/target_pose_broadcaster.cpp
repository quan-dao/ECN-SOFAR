//Cpp
#include <sstream>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>

//ROS
#include "ros/ros.h"

// Include here the ".h" files corresponding to the topic types you use.
#include <tf/transform_broadcaster.h>

// You may have a number of globals here.
//...

// Callback functions...

void myFirstCallback(/* Define here the variable which hold the message */){
    // ... Callback function code
}


int main (int argc, char** argv)
{

    //ROS Initialization
    ros::init(argc, argv, "right_pose_broadcaster");

    // Define your node handles
    ros::NodeHandle nh;

    // Read the node parameters if any
    // ...

    // Declare your node's subscriptions and service clients
    // ...

    // Declare you publishers and service servers
    tf::TransformBroadcaster br;

    // Define target frame w.r.t right_hand
    tf::Transform transform;
    tf::Quaternion q;
    q.setRPY(0, M_PI, 0);
    transform.setOrigin(tf::Vector3(0, 0, 0.25));
    transform.setRotation(q);

    ros::Rate rate(10);   // Or other rate.
    while (ros::ok()){
        ros::spinOnce();

        // Your node's code goes here.
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "right_hand", "/gr_13_left_target"));


        rate.sleep();
    }
}
