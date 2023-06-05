#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


class VideoSubscriber : public rclcpp::Node
{
public:
  VideoSubscriber()
  : Node("video_subscriber")
  {
    subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
      "video1", 10, std::bind(&VideoSubscriber::image_callback, this, std::placeholders::_1));
  }

private:
  void image_callback(const sensor_msgs::msg::Image::SharedPtr msg) 
  {

    rclcpp::WallRate loop_rate(20.0);
    Mat frame(msg->height, msg->width, CV_8UC3, const_cast<unsigned char*>(msg->data.data()), msg->step);
    Mat frame2;
    cvtColor(frame, frame2, COLOR_BGR2GRAY);
    //rclcpp::spin_some(Node);
    loop_rate.sleep();
    imshow("video_subscriber", frame);
    char ch = cv::waitKey(10);
    if(ch == 27) 
    {
      rclcpp::shutdown();
    }
  }
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<VideoSubscriber>());
  rclcpp::shutdown();
  return 0;
}