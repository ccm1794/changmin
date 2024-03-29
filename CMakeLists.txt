cmake_minimum_required(VERSION 3.5)
project(vision)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(OpenCV 4 REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)

add_executable(fusion_cam src/fusion_cam.cpp)
ament_target_dependencies(fusion_cam rclcpp std_msgs sensor_msgs cv_bridge image_transport OpenCV)

add_executable(simple_cam src/simple_cam.cpp)
ament_target_dependencies(simple_cam rclcpp std_msgs sensor_msgs cv_bridge image_transport OpenCV)

add_executable(cam_class src/cam_class.cpp)
ament_target_dependencies(cam_class rclcpp std_msgs sensor_msgs cv_bridge image_transport OpenCV)

add_executable(sub_image src/sub_image.cpp)
ament_target_dependencies(sub_image rclcpp std_msgs sensor_msgs cv_bridge image_transport OpenCV)

install(TARGETS
fusion_cam
simple_cam
cam_class
sub_image
DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY
  launch
  param
  DESTINATION share/${PROJECT_NAME}/
)

  

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
