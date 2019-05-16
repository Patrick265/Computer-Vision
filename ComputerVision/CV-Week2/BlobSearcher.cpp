#include "BlobSearcher.h"
cv::SimpleBlobDetector::Params params;


void setDetector() {
		params.maxThreshold = 255;
		params.minThreshold = 0;

		params.filterByColor = false;
		params.filterByArea = false;
		params.filterByCircularity = false;
		params.filterByConvexity = false;
		params.filterByInertia = false;
}

std::vector<cv::KeyPoint> find(cv::Mat image) {
		setDetector();
		cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
		std::vector<cv::KeyPoint> myBlobs;
		detector->detect(image, myBlobs);
		std::cout << "Size of blobs" << myBlobs.size() << std::endl;
		return myBlobs;
}