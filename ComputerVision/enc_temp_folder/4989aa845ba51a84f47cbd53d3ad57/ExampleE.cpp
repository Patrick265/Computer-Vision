#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>

#include "ExampleE.h"
#include "blobdetectionavans.h"

int runExampleE(int argc, char** argv) {
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

	//////////////////////////////////////////////////////////////////////////

	// Grijswaarde afbeelding thresholden
	cv::Mat binaryImage;
	int thresholdvalue = 200;
	threshold(gray_image, binaryImage, thresholdvalue, 1, cv::THRESH_BINARY_INV);
	cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
	imshow("Original", gray_image);
	cv::waitKey(0);

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
	show16SImageStretch(labeledImage, "Labeled Image");

	imwrite("C:\\Users\\timde\\Periode 2.4\\Vision\\Computer - Vision\\ComputerVision\\res\\labeledImage.jpg", labeledImage);

	// labelBLOBsInfo: met deze functie kun je ook BLOBs labelen. De functie geeft van 
	// alle BLOBs de volgende informatie terug:
	// - coordinaten van het eerste pixel van de BLOB
	// - coordinaten van het zwaartepunt van de BLOB.
	// - area van de BLOB
	cv::Mat labeledImage2;
	std::vector<cv::Point2d *> firstpixelVec2;
	std::vector<cv::Point2d *> posVec2;
	std::vector<int> areaVec2;
	labelBLOBsInfo(binary16S, labeledImage2, firstpixelVec2, posVec2, areaVec2);
	show16SImageStretch(labeledImage2, "Labeled Image 2");

	std::cout << std::endl << "*******************************************" << std::endl << std::endl;

	// Toon alle informatie in de console 
	std::cout << "Aantal gevonden BLOBs = " << firstpixelVec2.size() << std::endl;
	for (int i = 0; i < firstpixelVec2.size(); i++) {
		std::cout << "BLOB " << i + 1 << std::endl;
		std::cout << "firstpixel = (" << firstpixelVec2[i]->x << "," << firstpixelVec2[i]->y << ")" << std::endl;
		std::cout << "centre = (" << posVec2[i]->x << "," << posVec2[i]->y << ")" << std::endl;
		std::cout << "area = " << areaVec2[i] << std::endl;
	}

	std::cout << std::endl << "*******************************************" << std::endl << std::endl;

	// Met de functie labelBLOBsInfo kun je ook een threshold instellen voor de oppervlakte
	// van de BLOBs.

	cv::Mat labeledImage3;
	std::vector<cv::Point2d *> firstpixelVec3;
	std::vector<cv::Point2d *> posVec3;
	std::vector<int> areaVec3;
	int minArea = 15000;
	int maxArea = 16000;
	labelBLOBsInfo(binary16S, labeledImage3,
		firstpixelVec3, posVec3, areaVec3, minArea, maxArea);
	show16SImageStretch(labeledImage3, "Labeled Image 3");

	std::cout << std::endl << "*******************************************" << std::endl << std::endl;

	// Toon alle informatie in de console 
	std::cout << "Aantal gevonden BLOBs = " << firstpixelVec3.size() << std::endl;
	for (int i = 0; i < firstpixelVec3.size(); i++) {
		std::cout << "BLOB " << i + 1 << std::endl;
		std::cout << "firstpixel = (" << firstpixelVec3[i]->x << "," << firstpixelVec3[i]->y << ")" << std::endl;
		std::cout << "centre = (" << posVec3[i]->x << "," << posVec3[i]->y << ")" << std::endl;
		std::cout << "area = " << areaVec3[i] << std::endl;
	}

	std::cout << std::endl << "*******************************************" << std::endl << std::endl;

	// Nog een manier om het aantal BLOBs te verkrijgen...
	std::cout << "Total number of BLOBs = " << maxPixelImage(labeledImage3) << std::endl;

	std::string pipo;
	std::cin >> pipo;

	return 0;

}