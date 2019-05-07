#include <opencv2/core/core.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv)
{
		cv::VideoCapture webcam(0);
		if (!webcam.open(0)) {
				return 0;
		}
		
		for (;;) {
				cv::Mat videoFrame;
				webcam >> videoFrame;
				
				if (videoFrame.empty()) {
						break;
				}
				//Showing frame in a window
				imshow("Computer Vision Week 2", videoFrame);


				//Press escape to exit program
				if (cv::waitKey(10) == 27) {
						break;
				}
		}
		return 0;
}
