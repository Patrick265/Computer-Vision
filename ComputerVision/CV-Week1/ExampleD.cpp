#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>

#include "ExampleD.h"

int runExampleD(int argc, char** argv) {
	// zie: project properties - configuration properties - debugging - command arguments
	if (argc != 2)
	{
		std::cout << "NB! Geef als command argument volledige padnaam van de imagefile mee" << std::endl;
		return -1;
	}
	else std::cout << "De imagefile = " << argv[1] << std::endl;

	// Lees de afbeelding in
	cv::Mat image;
	image = cv::imread(argv[1], cv::IMREAD_COLOR);
	if (!image.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// De afbeelding converteren naar een grijswaarde afbeelding
	cv::Mat gray_image;
	cvtColor(image, gray_image, cv::COLOR_RGB2GRAY);

	// Converteren naar grijswaarde afbeelding
	std::cout << "Imagefile: " << argv[1] << " met succes geconverteerd naar grijswaarde beeld." << std::endl;

	// Grijswaarde afbeelding thresholden
	cv::Mat binaryx;
	threshold(gray_image, binaryx, 100, 1, cv::THRESH_BINARY);

	cv::namedWindow("Grijswaarde Beeld", cv::WINDOW_AUTOSIZE);
	imshow("Grijswaarde Beeld", gray_image);
	cv::waitKey(0);

	cv::namedWindow("Threshold Binary", cv::WINDOW_AUTOSIZE);
	imshow("Threshold Binary", binaryx * 255);
	cv::waitKey(0);

	// Saven op disk om het resultaat in VisionLab nader te kunnen bekijken
	imwrite("C:\\Users\\timde\\Periode 2.4\\Vision\\Computer-Vision\\ComputerVision\\res\\binairbeeld.jpg", binaryx*255);

	//waitKey(0);
}