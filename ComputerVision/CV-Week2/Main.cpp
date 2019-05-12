#include <opencv2/core/core.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "BlobSearcher.h"

//Threshold value that we use
#define THRESHOLDVAL 240

cv::Mat binaryVideoFrame;
cv::Mat grayScaleVideoFrame;
cv::Mat videoFrame;
cv::Mat erosionDst;
int erosionAmount = 1;


void erosionCallback(int, void *) {
		// Changing the erosion amount for the for loop depending on the trackbar
		erosionAmount = cv::getTrackbarPos("Erosion", "Erosion Window");
}
/*
* Using erosion to make the white parts smaller
*/
void erosion() {
		//Making a structeringelement of a size 7x7
		cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7,7));
		
		// Looping the amount of erosionamount, the amount is changed by the trakcbar
		for (int i = 0; i < erosionAmount; i++) {
				// eroding the binary video frame
				cv::erode(binaryVideoFrame, erosionDst, element);
				// Showing the erode
				imshow("Erosion Window", erosionDst);
				binaryVideoFrame = erosionDst;
		}
		// Releasing the getstructeringElement
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
		// Using the webcame
		cv::VideoCapture webcam(0);
		if (!webcam.open(0)) {
				return 0;
		}
		// Creating 2 windows one for the raw video
		cv::namedWindow("Computer Vision Week 2", cv::WINDOW_AUTOSIZE);
		// Second windows is for the binary video frame with erosion
		cv::namedWindow("Erosion Window", cv::WINDOW_AUTOSIZE);
		
		// Creating a trackbar
		cv::createTrackbar("Erosion", "Erosion Window", &erosionAmount, 15, erosionCallback);
		while(true) {
				// Putting webcam frame into a picture element
				webcam >> videoFrame;
				
				if (videoFrame.empty()) {
						break;
				}
				//Showing frame in a window
				imshow("Computer Vision Week 2", videoFrame);
				showThresholdImage();
				
				erosion();

				
				int key = cv::waitKey(10);
				// If key equals 27 (ESC) then break program
				if (key == 27) {
						return 0;
				}
				
				// If key equals 32 (SPACE) or 13 (ENTER) Continue with blobdetection
				if (key == 32 || key == 13) {
						break;
				}
				// Releasing all Mat variables
				videoFrame.release();
				binaryVideoFrame.release();
				grayScaleVideoFrame.release();

		}
		
		// Destroying current existing windows
		cv::destroyAllWindows();
		
		// New picture
		cv::Mat blobKeyPoints;
		// Find all keypoints with blobdetection
		std::vector<cv::KeyPoint> keypoints = find(erosionDst);
		std::cout << "Amount of objects found" << keypoints.size() << std::endl;
		// Drawing the keypoints
		cv::drawKeypoints(erosionDst, keypoints, blobKeyPoints, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		int i = 1;

		// Drawing the text of the keypoints amount
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