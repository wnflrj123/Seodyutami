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
	DisparityMapMaker maker(leftImagePath, rightImagePath, 8, 16, SIMDintrinsicSSDtype::getInstance());

	TimePrinter time;
	IplImage* disparityMap(maker.getDisparityMapPtr());
	time.end();

	cvSaveImage("matching.png", disparityMap);
	showImageAndStop(disparityMap);
	maker.deleteMap();

	return 0;
}