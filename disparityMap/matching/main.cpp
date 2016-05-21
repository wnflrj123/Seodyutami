#include <iostream>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "disparityMapMaker.h"
#include "timePrinter.h"
#include "segmentMarkersMaker.h"

void showImageAndStop(IplImage* image) {
	cvShowImage("test", image);
	cvWaitKey(0);
	cvDestroyWindow("test");
}

int main() {
	IplImage* leftGrayImage = cvLoadImage("tsukuba_l.png", 0);
	IplImage* leftColorImage = cvLoadImage("tsukuba_l.png", 1);
	IplImage* rightGrayImage = cvLoadImage("tsukuba_r.png", 0);

	DisparityMapMaker disparityMapMaker(2, 16, SSDtype::getInstance());
	SegmentMarkersMaker segmentMarkersMaker(8);

	TimePrinter time;
	std::shared_ptr<IplImage> disparityMap(disparityMapMaker.getDisparityMapPtr(leftGrayImage, rightGrayImage));
	std::shared_ptr<CvMat> segmentMarkers(segmentMarkersMaker.getSegmentMarkers(leftColorImage));
	disparityMap = segmentMarkersMaker.getSegmentedDisparityMap(disparityMap.get(), segmentMarkers.get());
	time.end();

	cvSaveImage("matching.png", disparityMap.get());
	showImageAndStop(disparityMap.get());
	
	return 0;
}