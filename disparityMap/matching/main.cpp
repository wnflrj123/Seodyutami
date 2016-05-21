#include <iostream>
#include <string>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "disparityMapMaker.h"
#include "timePrinter.h"

void showImageAndStop(IplImage* image) {
	cvShowImage("SSD test", image);
	cvWaitKey(0);
	cvDestroyWindow("SSD test");
}

int main() {
	const std::string leftImagePath = "tsukuba_l.png";
	const std::string rightImagePath = "tsukuba_r.png";
	IplImage* leftImage = cvLoadImage(leftImagePath.c_str(), 0);
	IplImage* rightImage = cvLoadImage(rightImagePath.c_str(), 0);

	DisparityMapMaker maker(8, 16, SIMDintrinsicSSDtype::getInstance());

	TimePrinter time;
	std::shared_ptr<IplImage> disparityMap(maker.getDisparityMapPtr(leftImage, rightImage));
	time.end();

	cvSaveImage("matching.png", disparityMap.get());
	showImageAndStop(disparityMap.get());
	
	return 0;
}