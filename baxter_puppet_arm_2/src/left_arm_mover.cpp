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
#include <tf/transform_listener.h>
#include <baxter_core_msgs/SolvePositionIK.h>
#include<sensor_msgs/JointState.h>
#include<baxter_core_msgs/JointCommand.h>
#include<geometry_msgs/PoseStamped.h>

// You may have a number of globals here.
//...

// Callback functions...

void myFirstCallback(/* Define here the variable which hold the message */){
    // ... Callback function code
}


int main (int argc, char** argv)
{

    //ROS Initialization
    ros::init(argc, argv, "left_arm_mover");

    // Define your node handles
    ros::NodeHandle nh;

    // Read the node parameters if any
    // ...

    // Declare your node's subscriptions and service clients
    // ...

    // Declare you publishers and service servers
    ros::Publisher pub_command = nh.advertise<baxter_core_msgs::JointCommand>("/robot/limb/left/joint_command", 1);

    ros::service::waitForService("/ExternalTools/left/PositionKinematicsNode/IKService");
    ros::ServiceClient ik_client = nh.serviceClient<baxter_core_msgs::SolvePositionIK>("/ExternalTools/left/PositionKinematicsNode/IKService");

    tf::TransformListener listener;
    uint32_t target_pose_id = 0;

    ros::Rate rate(10);   // Or other rate.
    while (ros::ok()){
        ros::spinOnce();

        // Your node's code goes here.
        tf::StampedTransform left_hand_target;
        bool target_received = false;  // set true if receive left_hand_target from tf listener
        try{
            listener.lookupTransform("/base", "/gr_13_left_target",
                                     ros::Time(0), left_hand_target);
            target_received = true;
        }
        catch (tf::TransformException ex){
            ROS_ERROR("%s",ex.what());
            ros::Duration(1.0).sleep();
        }

        if(target_received){
            ROS_INFO("Received target pose of left hand");
            // extract position
            tf::Vector3 target_posi = left_hand_target.getOrigin();
            ROS_INFO_STREAM("Target position:["<<target_posi.getX()<<",\t"<<target_posi.getY()<<",\t"<<target_posi.getZ()<<"]");

            // extract orientation
            tf::Quaternion target_ori = left_hand_target.getRotation();
            ROS_INFO_STREAM("Target orientation:["<<target_ori.x()<<",\t"<<target_ori.y()<<",\t"<<target_ori.z()<<",\t"<<target_ori.w()<<"]");

            // define target frame according to ik service format
            geometry_msgs::PoseStamped ik_target_pose;
            ik_target_pose.pose.position.x = target_posi.getX();
            ik_target_pose.pose.position.y = target_posi.getY();
            ik_target_pose.pose.position.z = target_posi.getZ();
            ik_target_pose.pose.orientation.x = target_ori.x();
            ik_target_pose.pose.orientation.y = target_ori.y();
            ik_target_pose.pose.orientation.z = target_ori.z();
            ik_target_pose.pose.orientation.w = target_ori.w();

            ik_target_pose.header.frame_id = "/base";
            ik_target_pose.header.stamp = ros::Time(0);
            ik_target_pose.header.seq = target_pose_id++;

            // define service request
            baxter_core_msgs::SolvePositionIK ik_srv_mess;
            ik_srv_mess.request.pose_stamp.push_back(ik_target_pose);
            ik_srv_mess.request.seed_mode = ik_srv_mess.request.SEED_AUTO;

            // call IK service
            if (ik_client.call(ik_srv_mess)){
                ROS_INFO("IK service returned %d solution", ik_srv_mess.response.joints.size());
                if(ik_srv_mess.response.result_type[0] == ik_srv_mess.response.RESULT_INVALID){
                    ROS_WARN("Recieved invalid solutions. Can't do anything");
                }else{
                    sensor_msgs::JointState ik_solution = ik_srv_mess.response.joints[0];
                    // define joint_command
                    baxter_core_msgs::JointCommand joint_command;
                    joint_command.mode = joint_command.POSITION_MODE;
                    for(int i = 0; i < ik_solution.name.size(); ++i){
                        joint_command.names.push_back(ik_solution.name[i]);
                        joint_command.command.push_back(ik_solution.position[i]);
                    }

                    // publish joint_command
                    pub_command.publish(joint_command);
                }
            }else{
                ROS_ERROR("Call to IK service failed");
            }

        }else{std::cout<<"Not recieved anything\n";}
        rate.sleep();
    }
}
