/* node name : sub_and_cap
 * subscribe topic : camera_image_raw
 * publish topic : 
터틀봇으로부터 이미지 섭 받아서 사진찍기 */

#include "opencv2/opencv.hpp"
#include <iostream>
#include <ros/ros.h>
#include <image_transport/image_transport.h> //img_trnasport should always be used to subscribe to and publish images
#include <sstream>
#include <cv_bridge/cv_bridge.h>

#define CAM_NUM 6

using namespace std;
using namespace cv::ml;
using namespace cv;

Mat frame;


int main(int argc, char **argv)
{

	int cam;
	cout << "input camera number: " << endl;
    cin >> cam;
    VideoCapture cap(cam);

    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);

	int i = 0;

	char mystr_1[10];
    char mystr_2[10];

	while (1)
	{
		cap >> frame;	

		imshow("video", frame);

		if (waitKey(1) == 'c') //사진 찍을 때마다 파일 번호 상승
		{
			i++;
			stringstream ss;
			ss << i;
			string str = ss.str();
			string filename = "image" + str + ".jpg";
			imwrite(filename, frame);
			printf("image saved");
		}
		char ch = cv::waitKey(10);
        if(ch == 27) break;
	}
	return 0;
}
