//Cpp
#include <sstream>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>

//ROS
#include "ros/ros.h"
#include <baxter_core_msgs/JointCommand.h>
#include <sensor_msgs/JointState.h>

// Include here the ".h" files corresponding to the topic types you use.
// ...

// You may have a number of globals here.
ros::Publisher pub;


// Callback functions...

void myFirstCallback(sensor_msgs::JointState joint_state){
    std::vector<std::string> flip_sign_list;
    flip_sign_list.push_back("s0");
    flip_sign_list.push_back("e0");
    flip_sign_list.push_back("w0");
    flip_sign_list.push_back("w2");

    if (joint_state.name.size() > 1) {
        // declare message type
        baxter_core_msgs::JointCommand data;
        data.mode = data.POSITION_MODE;

        for(int i = 0; i < joint_state.name.size(); ++i) {
            std::string name = joint_state.name[i];
            if (name.find("right") != std::string::npos) {
                // check if name containts right letter
                //std::cout << "name = " << name << "\n";
                std::string left_name = name.replace(0, 5, "left");
                data.names.push_back(left_name);

                // check if name belong to flip_sign_list
                std::string sub_name = name.substr(name.size()-2, name.size()-1);
                int h = 0;
                bool found = false;
                while (h < flip_sign_list.size() && !found){
                    if(flip_sign_list[h] == sub_name){found = true;}
                    h += 1;
                }

                if(found)
                {
                  data.command.push_back(-joint_state.position[i]);
                } else {data.command.push_back(joint_state.position[i]);}
            }
        }

        // publish
        pub.publish(data);
    }
}


int main (int argc, char** argv)
{

    //ROS Initialization
    ros::init(argc, argv, "slave_arm_node");

    // Define your node handles
    ros::NodeHandle nh;

    // Read the node parameters if any
    // ...

    // Declare your node's subscriptions and service clients
    ros::Subscriber sub = nh.subscribe("/robot/joint_states", 10, myFirstCallback);


    // Declare you publishers and service servers
    pub = nh.advertise<baxter_core_msgs::JointCommand>("/robot/limb/left/joint_command", 1);

    ros::Rate rate(10);   // Or other rate.
    while (ros::ok()){
        ros::spinOnce();

        // Your node's code goes here.

        rate.sleep();
    }
}
