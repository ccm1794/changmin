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

#define CAM_NUM 2

class VideoPublisher : public rclcpp::Node
{
public:
  VideoPublisher()
  : Node("Video_Publisher"), count_(0)
  {
    capture_ = cv::VideoCapture(CAM_NUM);
    if (!capture_.isOpened())
    {
      RCLCPP_ERROR(get_logger(), "Failed to open camera.");
      return;
    }

    publisher_ = this->create_publisher<sensor_msgs::msg::Image>("video", 10);

    timer_ = this->create_wall_timer(
      50ms, std::bind(&VideoPublisher::timer_callback, this));

    
  }

private:
  void timer_callback()
  {
    
    cv::Mat frame;
    capture_ >> frame;
    resize(frame, frame, Size(640,480));
    if (frame.empty())
    {
      RCLCPP_ERROR(get_logger(), "Failed to capture frame.");
      return;
    }

    imshow("frame", frame);
    waitKey(10);

    msg_ = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame).toImageMsg();
    // Publish the message
    publisher_->publish(*msg_.get());    
    RCLCPP_INFO(this->get_logger(), "Image %ld published", count_);

    count_++;
  }

  // void timer_callback()
  // {
  //   auto message = std_msgs::msg::String();
  //   message.data = "image  " + std::to_string(count_++);
  //   RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
  // }

  cv::VideoCapture capture_;
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;  
  sensor_msgs::msg::Image::SharedPtr msg_;
  size_t count_;
};

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<VideoPublisher>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}