#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "blobdetectionavans.h"

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

void showAreas();
void init();

/// Globale Variabelen voor de slider 
int threshold_slider_area = 0;
int area_slider_min = 0;
int area_slider_max = 10000;

int areaValueMin = 0;
int areaValueMax = 10000;

cv::Mat gray_image_area;
cv::Mat binaryx_area;

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

	showAreas();

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

	showAreas();

	// nieuwe resultaat tonen
	//imshow("binair beeld", binaryx_area * 255);
}

int runApplication1(int argc, char** argv) {
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
	cvtColor(image, gray_image_area, cv::COLOR_RGB2GRAY);

	// Converteren naar grijswaarde afbeelding
	std::cout << "Imagefile: " << argv[1] << " met succes geconverteerd naar grijswaarde beeld." << std::endl;

	/// Initialisatie slider waarde
	cv::namedWindow("Area finder", cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("Area min", "Area finder", &area_slider_min, area_slider_max, on_trackbar_area_min);
	cv::createTrackbar("Area max", "Area finder", &area_slider_max, area_slider_max, on_trackbar_area_max);

	imshow("Area finder", image);
	init();
	cv::waitKey(0);
}

void init() {
	// Grijswaarde afbeelding thresholden
	cv::Mat binaryImage;
	int thresholdvalue = 200;
	threshold(gray_image_area, binaryImage, thresholdvalue, 1, cv::THRESH_BINARY_INV);



	// Alvorens bewerkingen uit te voeren op het beeld converteren we deze
	// naar een Mat object met grotere diepte (depth), t.w. 16 bits signed
	cv::Mat binary16S;

	std::cout << "Entered" << std::endl;
	binaryImage.convertTo(binary16S, CV_16S);

	// functie labelBLOBs doet hetzelfde als Label Blobs in VisionLab; input is een
	// binair beeld. Output is een image waarin alle pixels van elke blob voorzien zijn van 
	// volgnummer.
	cv::Mat labeledImage;
	std::cout << "Total number of BLOBs " << labelBLOBs(binary16S, labeledImage) << std::endl;

	// functie show16SImageStretch beeld elke image af op 0 - 255 zodat je vrijwel altijd
	// wel een redelijke view krijgt op de data.
	// (show16SImageClip laat alle pixels met een waarde tussen 0 en 255 zien. Waardes onder 0
	// worden op 0 afgebeeld, waardes boven 255 worden op 255 afgebeeld.)
	//show16SImageStretch(labeledImage, "Labeled Image");


	// labelBLOBsInfo: met deze functie kun je ook BLOBs labelen. De functie geeft van 
	// alle BLOBs de volgende informatie terug:
	// - coordinaten van het eerste pixel van de BLOB
	// - coordinaten van het zwaartepunt van de BLOB.
	// - area van de BLOB
	cv::Mat labeledImage2;
	
	labelBLOBsInfo(binary16S, labeledImage2, firstpixelVec2, posVec2, areaVec2);

}

void showAreas() {

	std::cout << std::endl << "*******************************************" << std::endl << std::endl;

	// Toon alle informatie in de console 
	std::cout << "Aantal gevonden BLOBs = " << firstpixelVec2.size() << std::endl;
	for (int i = 0; i < firstpixelVec2.size(); i++) {
		if (areaVec2[i] >= areaValueMin && areaVec2[i] <= areaValueMax) {
			std::cout << "area = " << areaVec2[i] << std::endl;
		}
		
	}

	std::cout << std::endl << "*******************************************" << std::endl << std::endl;
}