# Intro
This repository contains my implementation of 4 lab sessions of the subject Software Architect for Robots at Ecole Centrale de Nantes. These labs are designed to be a medium for students to learn the basis of ROS. The subject of this lab is to program the Baxter robot so that it will operate according to some predefined manner.

# Lab 2 - Puppet Arm

Learning points:
* ROS Node
* Publish and Subscrib to topics

This lab's goal to program Baxter's left arm in such a way that it follow the right arm symmetrically. In detail, an operator will move Baxter's right hand, and the program has to make left arm follow that motion so that left arm is symmetric to right arm with respect to Baxter's sagittal plane.

To implement this, a `ROS Node` is written to subscribe to right hand's joints position and infer the position of the associate joints of left hand. The code of this is stored in *baxter_puppet_arm* folder

The result of this lab is shown in the video below.
<figure class="video_container">
  <iframe src="https://www.youtube.com/watch?v=0cLE0i6XuB4" frameborder="0" allowfullscreen="true"> </iframe>
</figure>

# Lab 3 - Puppet Arm

Learning points:
* TF package (use broadcaster & listener)
* ROS Service

Similar to lab 2, this lab attempts to control Baxter's left hand in accordance with the motion of its right hand. However, the left hand needs to move so that the end-effector frame of left hand remains a certain distance from right hand end-effector frame, and left end-effector's z-axis is in reverse direction of right end-effector z-axis.

To achieve that target, 2 nodes are written. One node uses `tf.TransformBroadcaster` to publish the target pose for left hand end-effector. 

Another node use `tf.TransformListener` to retrieve the left end-effector target pose. This node also calls the Baxter Inverse Kinematic service (defined in `baxter_common` package) to find the joints position which can help the left end-effector reach its target pose.

The code of this is stored in *baxter_puppet_arm_2* folder. The result of this lab is shown in the video below.
<figure class="video_container">
  <iframe src="https://www.youtube.com/watch?v=dxpa21cINzo" frameborder="0" allowfullscreen="true"> </iframe>
</figure>