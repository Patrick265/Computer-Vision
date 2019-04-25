#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "blobdetectionavans.h"

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

void showAreas();
void init();
void test(char** argv);

cv::Ptr<cv::SimpleBlobDetector> detector;
cv::Mat blobImg;
cv::SimpleBlobDetector::Params params;
std::vector<cv::KeyPoint> myBlobs;
cv::Mat inputImg;

/// Globale Variabelen voor de slider 
int threshold_slider_area = 0;
int area_slider_min = 0;
int area_slider_max = 10000;

int areaValueMin = 0;
int areaValueMax = 10000;

cv::Mat image;
cv::Mat firstImage;

cv::Mat gray_image_area;
cv::Mat binaryx_area;

cv::Mat labeledImage;
std::vector<cv::Point2d *> firstpixelVec2;
std::vector<cv::Point2d *> posVec2;
std::vector<int> areaVec2;

// Callback functie voor afhandeling trackbar events
void on_trackbar_area_min(int, void*)
{
	


	areaValueMin = cv::getTrackbarPos("Area min", "Area finder");
	// waarde ophalen van de slider
	if (areaValueMin >= areaValueMax && areaValueMin != 100) {
		areaValueMax = areaValueMin + 1;
		cv::setTrackbarPos("Area max","Area finder", areaValueMax);
	}

	params.minDistBetweenBlobs = 1.0;    //Minimum 1 pixel between blobs
	params.filterByArea = true;            //Checking for area
	params.filterByColor = false;        //We're doing a binary detection, we don't want color
	params.minArea = areaValueMin;        //Minimum value of the area
	params.maxArea = areaValueMax;        //Maximum value of the area
	params.thresholdStep = 100;            //Slider steps
	params.blobColor = 0;                //Color we're checking for
	params.filterByCircularity = true;    //We dont check for circularity
	params.filterByInertia = false;        //We dont check for Intertia
	params.filterByConvexity = true;    //We dont check for convexity
	params.minConvexity = 0.5;
	params.minCircularity = 0.15;

	//Creating a detector with the settings above
	detector = cv::SimpleBlobDetector::create(params);

	// nieuwe resultaat tonen
	//imshow("binair beeld", binaryx_area * 255);
}

// Callback functie voor afhandeling trackbar events
void on_trackbar_area_max(int, void*)
{
	areaValueMax = cv::getTrackbarPos("Area max", "Area finder");
	// waarde ophalen van de slider
	if (areaValueMax <= areaValueMin && areaValueMax != 0) {
		areaValueMin = areaValueMax - 1;
		cv::setTrackbarPos("Area min","Area finder", areaValueMin);
	}

	params.minDistBetweenBlobs = 1.0;    //Minimum 1 pixel between blobs
	params.filterByArea = true;            //Checking for area
	params.filterByColor = false;        //We're doing a binary detection, we don't want color
	params.minArea = areaValueMin;        //Minimum value of the area
	params.maxArea = areaValueMax;        //Maximum value of the area
	params.thresholdStep = 100;            //Slider steps
	params.blobColor = 0;                //Color we're checking for
	params.filterByCircularity = true;    //We dont check for circularity
	params.filterByInertia = false;        //We dont check for Intertia
	params.filterByConvexity = true;    //We dont check for convexity
	params.minConvexity = 0.5;
	params.minCircularity = 0.15;

	//Creating a detector with the settings above
	detector = cv::SimpleBlobDetector::create(params);

	// nieuwe resultaat tonen
	//imshow("binair beeld", binaryx_area * 255);
}

int runApplication1(int argc, char** argv) {

	// Lees de afbeelding in
	
	image = cv::imread(argv[1], cv::IMREAD_COLOR);
	if (!image.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// De afbeelding converteren naar een grijswaarde afbeelding
	cvtColor(image, gray_image_area, cv::COLOR_RGB2GRAY);

	// Converteren naar grijswaarde afbeelding
	std::cout << "Imagefile: " << argv[1] << " met succes geconverteerd naar grijswaarde beeld." << std::endl;


	/// Initialisatie slider waarde
	cv::namedWindow("Area finder", cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("Area min", "Area finder", &area_slider_min, area_slider_max, on_trackbar_area_min);
	cv::createTrackbar("Area max", "Area finder", &area_slider_max, area_slider_max, on_trackbar_area_max);

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

	//Creating a detector with the settings above
	detector = cv::SimpleBlobDetector::create(params);

	while (true) {
		//Detecting the blobs
		detector->detect(gray_image_area, myBlobs);

		std::cout << "min,max : " << areaValueMin << ", " << areaValueMax << std::endl;

		//Drawing keypoints (red circles)
		drawKeypoints(gray_image_area, myBlobs, blobImg, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		cv::imshow("Area finder", blobImg);

		
		cv::waitKey(5);
	}
	cv::waitKey(0);
}

