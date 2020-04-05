cmake_minimum_required(VERSION 2.8.3)
project(baxter_puppet_arm_2)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  baxter_core_msgs
  geometry_msgs
  roscpp
  std_msgs
  tf
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES baxter_lab3
   CATKIN_DEPENDS baxter_core_msgs geometry_msgs roscpp std_msgs tf
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(target_pose_broadcaster src/target_pose_broadcaster)
add_executable(left_arm_mover src/left_arm_mover)

target_link_libraries(target_pose_broadcaster
  ${catkin_LIBRARIES}
)

target_link_libraries(left_arm_mover
  ${catkin_LIBRARIES}
)

install(TARGETS target_pose_broadcaster
   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(TARGETS left_arm_mover
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
