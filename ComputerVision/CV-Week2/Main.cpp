#include <opencv2/core/core.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

#define THRESHOLDVAL 240
#define EROSIONAMOUNT 50

cv::Mat binaryVideoFrame;
cv::Mat grayScaleVideoFrame;
cv::Mat videoFrame;


/*
* Using erosion to make the white parts smaller
*/
void erosion() {
		cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3), cv::Point(-1, -1));
		cv::Mat erosionDst;
		for (int i = 0; i < EROSIONAMOUNT; i++) {
				cv::erode(binaryVideoFrame, erosionDst, element);
				imshow("Erosion", erosionDst * 255);
				binaryVideoFrame = erosionDst;
		}
}
void showThresholdImage()
{
		//Converting to grayscale
		cvtColor(videoFrame, grayScaleVideoFrame, cv::COLOR_RGB2GRAY);
		// Adding threshold to the grayscale image
		threshold(grayScaleVideoFrame, binaryVideoFrame, THRESHOLDVAL, 1, cv::THRESH_BINARY);
}

int main(int argc, char** argv)
{
		cv::VideoCapture webcam(0);
		if (!webcam.open(0)) {
				return 0;
		}
		
		while(true) {
				webcam >> videoFrame;
				
				if (videoFrame.empty()) {
						break;
				}
				//Showing frame in a window
				cv::namedWindow("Computer Vision Week 2", cv::WINDOW_AUTOSIZE);
				imshow("Computer Vision Week 2", videoFrame);
				showThresholdImage();
				
				erosion();

				//Press escape to exit program
				if (cv::waitKey(10) == 27) {
						break;
				}
				videoFrame.release();
				binaryVideoFrame.release();
				grayScaleVideoFrame.release();

		}
		return 0;
}