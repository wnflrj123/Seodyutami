#include <iostream>
#include <string>
#include "matching.h"
#include "timePrinter.h"

void showImageAndStop(IplImage* image) {
	cvShowImage("SSD test", image);
	cvWaitKey(0);
	cvDestroyWindow("SSD test");
}

int main() {
	const std::string leftImagePath = "tsukuba_l.png";
	const std::string rightImagePath = "tsukuba_r.png";
	DisparityMapMaker maker(leftImagePath, rightImagePath, 3, 16, new SIMDintrinsicSSDtype());

	TimePrinter time;
	IplImage* disparityMap(maker.getDisparitMapPtr());
	time.end();

	cvSaveImage("matching.png", disparityMap);
	showImageAndStop(disparityMap);

	return 0;
}