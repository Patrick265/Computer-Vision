

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>

/// Globale Variabelen voor de slider 
const int threshold_slider_max = 255;
int threshold_slider = 100;
int thresholdvalue = 100;

cv::Mat gray_image;
cv::Mat binaryx;

// Callback functie voor afhandeling trackbar events
void on_trackbar(int, void*)
{
	// waarde ophalen van de slider
	thresholdvalue = threshold_slider;

	// met verkregen waarde een operatie uitvoeren
	threshold(gray_image, binaryx, thresholdvalue, 1, cv::THRESH_BINARY);

	// nieuwe resultaat tonen
	imshow("binair beeld", binaryx * 255);
}

int runExampleC(int argc, char** argv) {
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
	cvtColor(image, gray_image, cv::COLOR_RGB2GRAY);

	// Converteren naar grijswaarde afbeelding
	std::cout << "Imagefile: " << argv[1] << " met succes geconverteerd naar grijswaarde beeld." << std::endl;

	cv::namedWindow("Grijswaarde Beeld", cv::WINDOW_AUTOSIZE);
	imshow("Grijswaarde Beeld", gray_image);
	cv::waitKey(0);

	/// Initialisatie slider waarde
	cv::namedWindow("binair beeld", cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("Threshold", "binair beeld", &threshold_slider, threshold_slider_max, on_trackbar);

	// Rechtstreeks aanroepen van de callback functie om het eerste beeld te krijgen
	on_trackbar(threshold_slider, 0);

	cv::waitKey(0);


}