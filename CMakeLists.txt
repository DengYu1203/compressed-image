cmake_minimum_required(VERSION 2.8.3)
project(project_name)  

add_compile_options(-std=c++11)


find_package(catkin REQUIRED COMPONENTS
pcl_ros
roscpp
cv_bridge
image_transport
)
find_package( OpenCV REQUIRED )
find_package(PCL REQUIRED COMPONENTS)
catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp pcl_ros
)

include_directories(
 include
 ${catkin_INCLUDE_DIRS}
 ${PCL_INCLUDE_DIRS}
 ${OpenCV_INCLUDE_DIRS}
)
link_directories(${PCL_LIBRARY_DIRS})

add_executable(publish_compressed_img src/publish_compressed_img.cpp)
target_link_libraries(publish_compressed_img
  ${catkin_LIBRARIES}
  ${PCL_LIBRARIES}
  ${OpenCV_LIBRARIES}
)

add_executable(subscribe_compressed_img src/subscribe_compressed_img.cpp)
target_link_libraries(subscribe_compressed_img
  ${catkin_LIBRARIES}
  ${PCL_LIBRARIES}
  ${OpenCV_LIBRARIES}
)
