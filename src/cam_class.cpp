#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;
///////////////////////////
// branch에서 커밋하기
//class공부해야함
class VideoPublisher : public rclcpp::Node
{
public:
  vector<double> mm;
public:
  VideoPublisher()
  : Node("cam_class")
  {
    RCLCPP_INFO(this->get_logger(), "------------ intialize ------------\n");

    //this->myMat(mm);
    //this->myMat = this->myMat.reshape(4, 4);
    //cout << this->myMat << endl;
    
    // this->declare_parameter("CameraMat", 0);
    // vector<double> CameraMat = this->get_parameter("CameraMat/data").get_value<vector<double>>();

    publisher_ = this->create_publisher<sensor_msgs::msg::Image>("video1", 10);
    publisher_2 = this->create_publisher<sensor_msgs::msg::Image>("video2", 10);

    videoCapture_.open(2);
    videoCapture_.set(CAP_PROP_FRAME_WIDTH, 640);
    videoCapture_.set(CAP_PROP_FRAME_HEIGHT, 480);

    videoCapture_2.open(4);
    videoCapture_2.set(CAP_PROP_FRAME_WIDTH, 640);
    videoCapture_2.set(CAP_PROP_FRAME_HEIGHT, 480);

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
      Mat frame2;
      videoCapture_ >> frame;
      videoCapture_2 >> frame2;

      if (frame.empty())
      {
        RCLCPP_ERROR(this->get_logger(), "Could not read frame from video stream");
        break;
      }
      if (frame2.empty())
      {
        RCLCPP_ERROR(this->get_logger(), "Could not read frame2 from video stream");
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


      sensor_msgs::msg::Image::UniquePtr image_msg2 = std::make_unique<sensor_msgs::msg::Image>();
      image_msg2->height = frame2.rows;
      image_msg2->width = frame2.cols;
      image_msg2->encoding = "bgr8";
      image_msg2->is_bigendian = false;
      image_msg2->step = static_cast<sensor_msgs::msg::Image::_step_type>(frame2.step);
      size_t size2 = frame2.step * frame2.rows;
      image_msg2->data.resize(size2);
      memcpy(&image_msg2->data[0], frame2.data, size2);

      publisher_2->publish(std::move(image_msg2));


      rclcpp::spin_some(this->get_node_base_interface());
    }
  }
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_2;
  VideoCapture videoCapture_;
  VideoCapture videoCapture_2;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<VideoPublisher>());
  rclcpp::shutdown();
  return 0;
}