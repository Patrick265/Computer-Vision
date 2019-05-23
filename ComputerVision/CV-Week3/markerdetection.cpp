
#include "markerdetection.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>


//Global variables OpenCV
cv::VideoCapture cap(0);
cv::Mat outputFrame, HsvFrame, thresholdImage;
cv::Ptr<cv::SimpleBlobDetector> detector;
cv::Mat circleImage;
cv::SimpleBlobDetector::Params params;
std::vector<cv::KeyPoint> myBlobs;
//Global variables HSV
const int maxHUEValue = 360 / 2;
const int maxValue = 255;
int lowH = 0, lowS = 200, lowV = 115;
int highH = 7, highS = maxValue, highV = maxValue;
//Global variables External
int areaSliderMin = 1000;
int areaSliderMax = 70000;
int exitLoop = 0;

//Initializes the params for the detector
void initializeBlobDetector() {
	params.minDistBetweenBlobs = 1.0;    //Minimum 1 pixel between blobs
	params.filterByArea = true;            //Checking for area
	params.filterByColor = false;        //We're doing a binary detection, we don't want color
	params.minArea = areaSliderMin;        //Minimum value of the area
	params.maxArea = areaSliderMax;        //Maximum value of the area
	params.thresholdStep = 100;            //Slider steps
	params.blobColor = 0;                //Color we're checking for
	params.filterByCircularity = true;    //We dont check for circularity
	params.filterByInertia = false;        //We dont check for Intertia
	params.filterByConvexity = true;    //We dont check for convexity
	params.minConvexity = 0.5;
	params.minCircularity = 0.15;

	//Creating a detector with the settings above
	detector = cv::SimpleBlobDetector::create(params);
}

//Starting to get the camera frames and manipuling them
void excecuteOpenCVDetection() {
	//Getting frames and checking the output
	cap >> outputFrame;
	if (outputFrame.empty()) {
		std::cerr << "Frame empty!" << std::endl;
		return;
	}
	//Flipping frame to get the right way up
	flip(outputFrame, outputFrame, 1);

	// Converting the outputframe to a hsvframe 
	cvtColor(outputFrame, HsvFrame, cv::COLOR_BGR2HSV);
	// Checking the HsvFrame for the correct range values (the low and high values from h,s and v) and creating the thresholdImage
	inRange(HsvFrame, cv::Scalar(lowH, lowS, lowV), cv::Scalar(highH, highS, highV), thresholdImage);

	//Detecting the blobs and putting them in myBlobs
	detector->detect(thresholdImage, myBlobs);

	//Creating a blank Mat image
	cv::Mat blank = cv::Mat::zeros(thresholdImage.size(), CV_8UC3);;
	//Drawing green circles around the centre of mass of the blobs and adding them to the blank image 
	//(circleImage is the output image with only the green circles)
	drawKeypoints(blank, myBlobs, circleImage, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	//Adding the circles to the original outputFrame
	outputFrame = outputFrame + circleImage;

	//Showing outputFrame and trackbars
	cv::imshow("circle image", outputFrame);
	cv::createTrackbar("LowH", "circle image", &lowH, 179);
	cv::createTrackbar("HighH", "circle image", &highH, 179);
	cv::createTrackbar("LowS", "circle image", &lowS, 255);
	cv::createTrackbar("HighS", "circle image", &highS, 255);

	if (cv::waitKey(5) == 32) {
		exitLoop = 1;
	}

}

int runMarkerDetection()
{
	//Creating a main window
	cv::namedWindow("circle image", cv::WINDOW_AUTOSIZE);
	//Setting trackbars to be able to manipulate the h and s values that the detector uses
	cv::createTrackbar("LowH", "circle image", &lowH, 179);
	cv::createTrackbar("HighH", "circle image", &highH, 179);
	cv::createTrackbar("LowS", "circle image", &lowS, 255);
	cv::createTrackbar("HighS", "circle image", &highS, 255);

	//Initializing the detector
	initializeBlobDetector();
	while (exitLoop == 0) {
			//Executing the detectionfunction
			excecuteOpenCVDetection();
	}
		return 0;
}
