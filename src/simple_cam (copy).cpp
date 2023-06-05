#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "std_msgs/msg/header.hpp"
#include <chrono>
#include <string>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.hpp>
#include <opencv2/opencv.hpp>
#include "std_msgs/msg/string.hpp"

using namespace std;
using namespace cv;
using namespace std::chrono_literals;

Mat frame1,frame2,frame3;

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("video_publisher");

  //rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher;
  auto publisher1 = node->create_publisher<sensor_msgs::msg::Image>("video1", 10);
  sensor_msgs::msg::Image::SharedPtr msg1;

  auto publisher2 = node->create_publisher<sensor_msgs::msg::Image>("video2", 10);
  sensor_msgs::msg::Image::SharedPtr msg2;

  auto publisher3 = node->create_publisher<sensor_msgs::msg::Image>("video3", 10);
  sensor_msgs::msg::Image::SharedPtr msg3;

  VideoCapture cap1(2);
  cap1.set(CAP_PROP_FRAME_WIDTH, 320);
  cap1.set(CAP_PROP_FRAME_HEIGHT, 240);

  VideoCapture cap2(4);
  cap2.set(CAP_PROP_FRAME_WIDTH, 320);
  cap2.set(CAP_PROP_FRAME_HEIGHT, 240);

  VideoCapture cap3(8);
  cap3.set(CAP_PROP_FRAME_WIDTH, 320);
  cap3.set(CAP_PROP_FRAME_HEIGHT, 240);

  // VideoCapture cap4(8);
  // cap4.set(CAP_PROP_FRAME_WIDTH, 160);
  // cap4.set(CAP_PROP_FRAME_HEIGHT, 120);

  rclcpp::WallRate loop_rate(10.0);

  while(rclcpp::ok())
  {
    cap1 >> frame1;
    cap2 >> frame2;
    cap3 >> frame3;
    //cap4 >> frame4;

    msg1 = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame1).toImageMsg();
    publisher1->publish(*msg1.get()); 
    msg2 = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame2).toImageMsg();
    publisher2->publish(*msg2.get()); 
    msg3 = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame3).toImageMsg();
    publisher3->publish(*msg3.get()); 

    rclcpp::spin_some(node);
    loop_rate.sleep();

    imshow("video1", frame1);
    imshow("video2", frame2);
    imshow("video3", frame3);
    // imshow("video4", frame4);

    char ch = cv::waitKey(10);
    if(ch == 27) break;
  }

}