#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "ExampleA.h"

int runExampleA(int argc, char** argv) {
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

	// Laat de afbeelding zien in een apart window
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	imshow("Display window", image);

	// Wacht op een muiskklik in het window van de afbeelding
	cv::waitKey(0);

	// Ruim alle aangemaakte windows weer op.
	cv::destroyAllWindows();

	return 0;
}