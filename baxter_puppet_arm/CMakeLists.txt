cmake_minimum_required(VERSION 2.8.3)
project(baxter_puppet_arm)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  baxter_core_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES move_joint
  CATKIN_DEPENDS roscpp std_msgs baxter_core_msgs
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(slave_arm_node src/slave_arm_node.cpp)
add_executable(slave_arm_node_velocity src/slave_arm_node_velocity.cpp)

target_link_libraries(slave_arm_node
  ${catkin_LIBRARIES}
)

target_link_libraries(slave_arm_node_velocity
  ${catkin_LIBRARIES}
)

install(TARGETS slave_arm_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
 )
install(TARGETS slave_arm_node_velocity
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
 )

