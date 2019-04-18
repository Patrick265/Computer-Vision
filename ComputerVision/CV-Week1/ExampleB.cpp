#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "ExampleB.h"

int runExampleB(int argc, char** argv) {
	// Controle of er een argument aan het programma is meegegeven.
	if (argc != 2)
	{
		std::cout << " Usage: display_image ImageToLoadAndDisplay" << std::endl;
		return -1;
	}

	// Mat is een class voor objecten waarin een afbeelding kan worden opgeslagen.
	cv::Mat image;

	// Lees de afbeelding in en sla deze op in image. 
	// De filenaam is het eerste argument dat meegegeven is bij aanroep van het programma.
	image = cv::imread(argv[1], cv::IMREAD_COLOR);

	// Controleer of alles goed is gegaan
	if (!image.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// Mat object aanmaken voor grijswaarde beeld
	cv::Mat gray_image;

	// Converteren van de ingelezen afbeelding naar een grijswaarde beeld.
	cv::cvtColor(image, gray_image, cv::COLOR_RGB2GRAY);

	// Opslaan van het grijswaarde beeld.11
	std::string padnaam = "C:\\Users\\timde\\Periode 2.4\\Vision\\Computer-Vision\\ComputerVision\\res\\Gray_ImageTest.jpg";
	imwrite(padnaam, gray_image);

	/****** Resultaten naar het scherm sturen ******/

	// Grijswaarde beeld tonen op het scherm
	cv::namedWindow("Gray image", cv::WINDOW_AUTOSIZE);
	imshow("Gray image", gray_image);
	cv::waitKey(0);

	// Gebruikersinformatie
	std::cout << "Imagefile: " << argv[1] << " met succes geconverteerd naar grijswaarde beeld." << std::endl;
	std::cout << "Grijswaarde beeld is opgeslagen: " << padnaam << std::endl;
	std::cout << "Druk op ENTER voor vervolg ==> " << std::endl;

	std::string dummy;
	std::getline(std::cin, dummy);

	return 0;
}