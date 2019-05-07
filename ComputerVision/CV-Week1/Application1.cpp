//Includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "opencv2/imgproc/imgproc.hpp" 
#include "blobdetectionavans.h"

//Global image variables
cv::SimpleBlobDetector::Params params;
cv::Ptr<cv::SimpleBlobDetector> detector;
std::vector<cv::KeyPoint> myBlobs;
cv::Mat blackImage;
cv::Mat blobImg;
cv::Mat threshImage;
cv::Mat image;
cv::Mat gray_image_area;

//Global variables 
int threshold_slider_area = 0;
int area_slider_min = 0;
int area_slider_max = 15000;
int areaValueMin = 0;
int areaValueMax = 15000;

//Local Functions
void setDetector() {
	params.minDistBetweenBlobs = 1.0;    //Minimum 1 pixel between blobs
	params.filterByArea = true;            //Checking for area
	params.filterByColor = false;        //We're doing a binary detection, we don't want color
	params.minArea = areaValueMin;        //Minimum value of the area
	params.maxArea = areaValueMax;        //Maximum value of the area
	params.thresholdStep = 100;            //Slider steps
	params.blobColor = 0;                //Color we're checking for
	params.filterByCircularity = false;    //We dont check for circularity
	params.filterByInertia = false;        //We dont check for Intertia
	params.filterByConvexity = false;    //We dont check for convexity
}

//Function resets the detector with new area params and redraws the image
void reEvaluateBlobs() {
	//Creating a detector with the settings above
	detector = cv::SimpleBlobDetector::create(params);

	//Detecting the blobs
	detector->detect(threshImage, myBlobs);

	//Drawing keypoints (red circles)
	drawKeypoints(blackImage, myBlobs, blobImg, cv::Scalar(255, 255, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	//Showing the new image
	cv::imshow("Area finder", blobImg);
}

//Callback for minimum area slider
void on_trackbar_area_min(int, void*)
{
	//Get value from slider
	areaValueMin = cv::getTrackbarPos("Area min", "Area finder");
	//Check if the min and max values don't surpass eachother
	if (areaValueMin >= areaValueMax && areaValueMin != 100) {
		areaValueMax = areaValueMin + 1;
		cv::setTrackbarPos("Area max","Area finder", areaValueMax);
	}
	//Reset detector and image
	setDetector();
	reEvaluateBlobs();
}

//Callback for maximum area slider
void on_trackbar_area_max(int, void*)
{
	//Get value from slider
	areaValueMax = cv::getTrackbarPos("Area max", "Area finder");
	//Check if the min and max values don't surpass eachother
	if (areaValueMax <= areaValueMin && areaValueMax != 0) {
		areaValueMin = areaValueMax - 1;
		cv::setTrackbarPos("Area min","Area finder", areaValueMin);
	}
	//Reset detector and image
	setDetector();
	reEvaluateBlobs();
}

void runApplication1(int argc, char** argv) {
	//Read image
	image = cv::imread(argv[1], cv::IMREAD_COLOR);
	if (!image.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
	}

	//Get grayvalue image
	cvtColor(image, gray_image_area, cv::COLOR_RGB2GRAY);

	//Creating a black image to draw on
	int thresholdvalue = 200;
	threshold(gray_image_area, blackImage, thresholdvalue, 1, cv::THRESH_BINARY_INV);
	cv::imshow("Normal Image", image);

	//Creating threshold image
	threshold(gray_image_area, threshImage, 240, 255, cv::THRESH_BINARY_INV);
	cv::imshow("thresh image", threshImage);

	//Initialize the trackbars
	cv::namedWindow("Area finder", cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("Area min", "Area finder", &area_slider_min, area_slider_max, on_trackbar_area_min);
	cv::createTrackbar("Area max", "Area finder", &area_slider_max, area_slider_max, on_trackbar_area_max);

	//Initialize detector and image for the first time
	setDetector();
	reEvaluateBlobs();

	cv::waitKey(0);
}

