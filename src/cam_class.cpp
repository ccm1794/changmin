#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

using namespace cv;
using namespace std;

// branch에서 커밋하기
//class공부해야함
class VideoPublisher : public rclcpp::Node
{
public:
  VideoPublisher()
  : Node("cam_class")
  {
    this->declare_parameter("CameraExtrinsicMat", vector<double>());
    vector<double> mm = this->get_parameter("CameraExtrinsicMat").as_double_array();
    Mat myMat(mm);
    myMat = myMat.reshape(4, 4);
    cout << myMat << endl;
    
    // this->declare_parameter("CameraMat", 0);
    // vector<double> CameraMat = this->get_parameter("CameraMat/data").get_value<vector<double>>();

    publisher_ = this->create_publisher<sensor_msgs::msg::Image>("video1", 10);

    videoCapture_.open(0);
    videoCapture_.set(CAP_PROP_FRAME_WIDTH, 320);
    videoCapture_.set(CAP_PROP_FRAME_HEIGHT, 240);

    if (!videoCapture_.isOpened())
    {
      RCLCPP_ERROR(this->get_logger(), "Could not open video stream");
      return;
    }
    publishVideoFrames();
  }
private:
  void publishVideoFrames()
  {
    while (rclcpp::ok())
    {
      Mat frame;
      videoCapture_ >> frame;

      if (frame.empty())
      {
        RCLCPP_ERROR(this->get_logger(), "Could not read frame from video stream");
        break;
      }

      sensor_msgs::msg::Image::UniquePtr image_msg = std::make_unique<sensor_msgs::msg::Image>();
      image_msg->height = frame.rows;
      image_msg->width = frame.cols;
      image_msg->encoding = "bgr8";
      image_msg->is_bigendian = false;
      image_msg->step = static_cast<sensor_msgs::msg::Image::_step_type>(frame.step);
      size_t size = frame.step * frame.rows;
      image_msg->data.resize(size);
      memcpy(&image_msg->data[0], frame.data, size);

      publisher_->publish(std::move(image_msg));
      rclcpp::spin_some(this->get_node_base_interface());
    }
  }
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
  VideoCapture videoCapture_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<VideoPublisher>());
  rclcpp::shutdown();
  return 0;
}