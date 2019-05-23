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


float jointErrors(sensor_msgs::JointState joint_state){
    //calculate the sum of differences between the joints of arms
    std::vector<float> leftState;
    std::vector<float> rightState;
    float error=0.0f;

    for(int i = 0; i < joint_state.name.size(); ++i) {
        //loop through joint state, if it finds variable starting with left or right
        //push back into vector containing the corresponding states
        std::string name = joint_state.name[i];
        if (name.find("right") != std::string::npos) {
            rightState.push_back(joint_state.position[i]);
        }
        if (name.find("left") != std::string::npos) {
            leftState.push_back(joint_state.position[i]);
        }
    }
    for(int i=0;i<leftState.size();++i){
        //add the absolute value of the joint differences
        error+=abs(leftState[i]-rightState[i]);
    }
    return error;

}

void myFirstCallback(sensor_msgs::JointState joint_state){
    baxter_core_msgs::JointCommand data;
    //create vector containing the joint name for which the signs need to be switched
    std::vector<std::string> flip_sign_list;
    flip_sign_list.push_back("s0");
    flip_sign_list.push_back("e0");
    flip_sign_list.push_back("w0");
    flip_sign_list.push_back("w2");

    float errorThreshold=10.0f;//threshold for the allowed error    NOT TUNED YET
    //only consider msgs that concern the complete state
    if (joint_state.name.size()>1){
        //if the error is larger than the threshold use the position mode
        if (jointErrors(joint_state)>errorThreshold){

            data.mode = data.POSITION_MODE;

            for(int i = 0; i < joint_state.name.size(); ++i) {
                std::string name = joint_state.name[i];
                // check if name containts the word 'right'
                if (name.find("right") != std::string::npos) {
                    //replace the part of the string with left and save it in diff string
                    std::string left_name = name.replace(0, 5, "left");
                    data.names.push_back(left_name);
                    // check if name belong to flip_sign_list
                    //for this we access the last strings in the name to check which joint it
                    //references to
                    std::string sub_name = name.substr(name.size()-2, name.size()-1);
                    int h = 0;
                    bool found = false;
                    //loop through list to check if it can be found in the list
                    //either we loop through entire list or we find the value
                    while (h < flip_sign_list.size() && !found){
                        if(flip_sign_list[h] == sub_name){found = true;}
                        h += 1;
                    }
                    //if we found the value in the list we push the command with a minus
                    //if not we push without
                    if(found)
                    {
                        data.command.push_back(-joint_state.position[i]);
                    } else {
                        data.command.push_back(joint_state.position[i]);
                    }
                }

            }


        }else{

            //if we are below the value we move in velocity mode

            data.mode = data.VELOCITY_MODE;

            for(int i = 0; i < joint_state.name.size(); ++i) {
                std::string name = joint_state.name[i];
                // check if name containts the word 'right'
                if (name.find("right") != std::string::npos) {
                    //replace right by left
                    std::string left_name = name.replace(0, 5, "left");
                    data.names.push_back(left_name);

                    // check if name belong to flip_sign_list
                    //for this we access the last strings in the name to check which joint it
                    //references to
                    std::string sub_name = name.substr(name.size()-2, name.size()-1);
                    int h = 0;
                    bool found = false;
                    while (h < flip_sign_list.size() && !found){
                        if(flip_sign_list[h] == sub_name){found = true;}
                        h += 1;
                    }

                    if(found)
                    {
                        data.command.push_back(-joint_state.velocity[i]);
                    } else {data.command.push_back(joint_state.// Read the node parameters if any
                                                   // ...
velocity[i]);}
                }

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
