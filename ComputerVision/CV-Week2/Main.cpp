#include <opencv2/core/core.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "BlobSearcher.h"

#define THRESHOLDVAL 240
cv::Mat binaryVideoFrame;
cv::Mat grayScaleVideoFrame;
cv::Mat videoFrame;
cv::Mat erosionDst;
int erosionAmount = 1;


void erosionCallback(int, void *) {
		erosionAmount = cv::getTrackbarPos("Erosion", "Erosion Window");
}
/*
* Using erosion to make the white parts smaller
*/
void erosion() {
		cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7,7));
		
		for (int i = 0; i < erosionAmount; i++) {
				cv::erode(binaryVideoFrame, erosionDst, element);
				imshow("Erosion Window", erosionDst);
				binaryVideoFrame = erosionDst;
		}

		element.release();
}
void showThresholdImage()
{
		//Converting to grayscale
		cvtColor(videoFrame, grayScaleVideoFrame, cv::COLOR_RGB2GRAY);
		// Adding threshold to the grayscale image
		threshold(grayScaleVideoFrame, binaryVideoFrame, THRESHOLDVAL, 255, cv::THRESH_BINARY);
}

int main(int argc, char** argv)
{
		cv::VideoCapture webcam(0);
		if (!webcam.open(0)) {
				return 0;
		}
		cv::namedWindow("Computer Vision Week 2", cv::WINDOW_AUTOSIZE);
		cv::namedWindow("Erosion Window", cv::WINDOW_AUTOSIZE);
		
		cv::createTrackbar("Erosion", "Erosion Window", &erosionAmount, 15, erosionCallback);
		while(true) {
				webcam >> videoFrame;
				
				if (videoFrame.empty()) {
						break;
				}
				//Showing frame in a window

				imshow("Computer Vision Week 2", videoFrame);
				showThresholdImage();
				
				erosion();

				int key = cv::waitKey(10);
				if (key == 27) {
						return 0;
				}
				

				if (key == 32 || key == 13) {
						break;
				}
				videoFrame.release();
				binaryVideoFrame.release();
				grayScaleVideoFrame.release();

		}
		

		cv::destroyAllWindows();
		
		cv::Mat blobKeyPoints;
		std::vector<cv::KeyPoint> keypoints = find(erosionDst);
		std::cout << "Amount of objects found" << keypoints.size() << std::endl;

		cv::drawKeypoints(erosionDst, keypoints, blobKeyPoints, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		int i = 1;
		for (cv::KeyPoint k : keypoints)
		{
				putText(blobKeyPoints, std::to_string(i), cv::Point(k.pt.x + 2, k.pt.y + 2), cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 255, 0), 2.0); 
				i++;
		}
		std::string name = "Blob counter, amount: " + std::to_string(keypoints.size());
		cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
		imshow(name, blobKeyPoints);
	
		cv::waitKey(0);
}