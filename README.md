# Software Architecture for Robots (SOFAR @ ECN
This repository contains my implementation of lab sessions of the subject Software Architect for Robots at Ecole Centrale de Nantes. These labs are designed to be a medium for students to learn the basis of *ROS*. The subject of this lab is to program the Baxter robot so that one arm is manually controlled while the other arm follows it in a predefine manner.

## Lab 2 - Symmetrical Follower

This lab's goal to program Baxter's left arm in such a way that it follow the right arm symmetrically as in the figure below.

![alt text](./img/Baxter_puppet_arm.gif)

The package of this for lab is located in `./baxter_puppet_arm`. To build this, it's essential to have the package [baxter_core_msgs](https://github.com/RethinkRobotics/baxter_common/tree/master/baxter_core_msgs)

To start controlling the robot, make sure Baxter robot is up and running, then launch either the node `slave_arm_node` or `slave_arm_node_velocity`

```
$ rosrun baxter_puppet_arm slave_arm_node
```

This node subscribes to right hand's joints position and infer the position of the associate joints of left hand then publishes the joint position to `/robot/limb/left/joint_command` topic.

## Lab 3 - Constant Distance Follower

In this lab, the program-controlled left arm is moved such that the end-effector frame of left hand remains a certain distance from right hand end-effector frame, and left end-effector's z-axis is in reverse direction of right end-effector z-axis. 

![alt text](./img/Baxter_puppet_arm_2.gif)

Similar to lab 2, to build lab 3's package (located in `./baxter_puppet_arm_2`), the `baxter_core_msgs` package is required.

To start controlling the robot, use the launch file `lab3.launch` after Baxter robot is up

```
$ roslaunch baxter_puppet_arm_2 lab3.launch
```

This launch file spins up 2 nodes 

* target_pose_broadcaster node
  - Construct the target pose for left hand end-effector from the end-effector's frame of the right hand and publish it uisng `tf.TransformBroadcaster`

* left_arm_mover node 
  - calls the Baxter Inverse Kinematic service to find the left arm's joints position which move the left end-effector to the target pose published above
